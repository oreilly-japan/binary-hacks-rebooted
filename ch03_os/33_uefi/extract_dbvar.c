/*
    git clone --depth 1 --branch vUDK2018 https://github.com/tianocore/edk2.git
    gcc -Werror -I edk2/MdePkg/Include/ -I edk2/MdePkg/Include/X64/ \
        -o extract_dbvar -lcrypto extract_dbvar.c
*/
#include <openssl/pkcs7.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// NULLの再定義警告を避ける
#undef NULL
#include <Uefi.h>

#include <Guid/ImageAuthentication.h>

int main(int argc, char **argv) {
  // データベースを読み込む
  FILE *DbVarFp;
  long DbVarSize;
  EFI_VARIABLE_AUTHENTICATION_2 *DbVar;
  DbVarFp = fopen(argv[1], "r");
  fseek(DbVarFp, 0L, SEEK_END);
  DbVarSize = ftell(DbVarFp);
  fseek(DbVarFp, 0L, SEEK_SET);
  DbVar = malloc(DbVarSize);
  fread(DbVar, DbVarSize, 1, DbVarFp);
  fclose(DbVarFp);

  // 署名とペイロードの位置と長さを求める
  // 実装参考:
  // https://github.com/tianocore/edk2/blob/vUDK2018/SecurityPkg/Library/AuthVariableLib/AuthService.c#L1793
  UINT32 SigSize, PayloadSize;
  UINT8 *SigData, *PayloadData;
  SigData = DbVar->AuthInfo.CertData;
  SigSize = DbVar->AuthInfo.Hdr.dwLength -
            offsetof(WIN_CERTIFICATE_UEFI_GUID, CertData);
  PayloadData = SigData + SigSize;
  PayloadSize =
      DbVarSize - DbVar->AuthInfo.Hdr.dwLength - sizeof(DbVar->TimeStamp);

  // 署名の先頭にContentInfoヘッダを付与する
  // 実装参考:
  // https://github.com/tianocore/edk2/blob/vUDK2018/CryptoPkg/Library/BaseCryptLib/Pk/CryptPkcs7Verify.c#L57
  const UINT8 Pkcs7SignedDataOid[9] = {0x2A, 0x86, 0x48, 0x86, 0xF7,
                                       0x0D, 0x01, 0x07, 0x02};
  UINT32 NewSigSize;
  UINT8 *NewSigData;
  NewSigSize = SigSize + 19;
  NewSigData = malloc(NewSigSize);
  NewSigData[0] = 0x30;
  NewSigData[1] = 0x82;
  NewSigData[2] = (NewSigSize - 4) >> 8;
  NewSigData[3] = (NewSigSize - 4) & 0xff;
  NewSigData[4] = 0x06;
  NewSigData[5] = 0x09;
  memcpy(NewSigData + 6, Pkcs7SignedDataOid, sizeof(Pkcs7SignedDataOid));
  NewSigData[15] = 0xA0;
  NewSigData[16] = 0x82;
  NewSigData[17] = SigSize >> 8;
  NewSigData[18] = SigSize & 0xff;
  memcpy(NewSigData + 19, SigData, SigSize);

  // ペイロードの先頭に変数の情報を付与する
  // 実装参考:
  // https://github.com/tianocore/edk2/blob/vUDK2018/SecurityPkg/Library/AuthVariableLib/AuthService.c#L1793
  UINT32 NewPayloadSize, VarNameSize;
  UINT8 *NewPayloadData, *CurrentPtr;
  VarNameSize = strlen(argv[4]) * 2;
  UINT8 *VarName = malloc(VarNameSize);
  for (int i = 0; i < VarNameSize; i += 2) {
    VarName[i + 0] = argv[4][i / 2];
    VarName[i + 1] = 0;
  }
  EFI_GUID VarGuid = EFI_IMAGE_SECURITY_DATABASE_GUID;
  UINT32 VarAttr = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS |
                   EFI_VARIABLE_BOOTSERVICE_ACCESS |
                   EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS |
                   EFI_VARIABLE_APPEND_WRITE;
  NewPayloadSize = PayloadSize + VarNameSize + sizeof(VarGuid) +
                   sizeof(VarAttr) + sizeof(DbVar->TimeStamp);
  NewPayloadData = malloc(NewPayloadSize);

  CurrentPtr = NewPayloadData;
  memcpy(CurrentPtr, VarName, VarNameSize);
  CurrentPtr += VarNameSize;
  memcpy(CurrentPtr, &VarGuid, sizeof(VarGuid));
  CurrentPtr += sizeof(VarGuid);
  memcpy(CurrentPtr, &VarAttr, sizeof(VarAttr));
  CurrentPtr += sizeof(VarAttr);
  memcpy(CurrentPtr, &DbVar->TimeStamp, sizeof(DbVar->TimeStamp));
  CurrentPtr += sizeof(DbVar->TimeStamp);
  memcpy(CurrentPtr, PayloadData, PayloadSize);

  // 署名とペイロードを書き出す
  FILE *SigFp, *PayloadFp;
  SigFp = fopen(argv[2], "w");
  fwrite(NewSigData, NewSigSize, 1, SigFp);
  fclose(SigFp);
  PayloadFp = fopen(argv[3], "w");
  fwrite(NewPayloadData, NewPayloadSize, 1, PayloadFp);
  fclose(PayloadFp);
}
