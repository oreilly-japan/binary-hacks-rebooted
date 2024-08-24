#define _GNU_SOURCE
#include <fcntl.h>
#include <linux/landlock.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <unistd.h>

#ifndef LANDLOCK_ACCESS_FS_TRUNCATE
#define LANDLOCK_ACCESS_FS_REFER (1ULL << 13)
#define LANDLOCK_ACCESS_FS_TRUNCATE (1ULL << 14)
#endif

int main(void) {
  // 拒否したいすべてのオペレーションを表すフラグ群を定義する
  uint64_t access_fs_read = LANDLOCK_ACCESS_FS_EXECUTE |
                            LANDLOCK_ACCESS_FS_READ_FILE |
                            LANDLOCK_ACCESS_FS_READ_DIR;
  uint64_t access_fs_write =
      LANDLOCK_ACCESS_FS_WRITE_FILE | LANDLOCK_ACCESS_FS_REMOVE_DIR |
      LANDLOCK_ACCESS_FS_REMOVE_FILE |
      LANDLOCK_ACCESS_FS_MAKE_CHAR | LANDLOCK_ACCESS_FS_MAKE_DIR |
      LANDLOCK_ACCESS_FS_MAKE_REG | LANDLOCK_ACCESS_FS_MAKE_SOCK |
      LANDLOCK_ACCESS_FS_MAKE_FIFO | LANDLOCK_ACCESS_FS_MAKE_BLOCK |
      LANDLOCK_ACCESS_FS_MAKE_SYM | LANDLOCK_ACCESS_FS_REFER |
      LANDLOCK_ACCESS_FS_TRUNCATE;

  // カーネルがサポートするLandlockのバージョンを確認する
  int abi = syscall(__NR_landlock_create_ruleset, NULL, 0,
                    LANDLOCK_CREATE_RULESET_VERSION);
  if (abi < 0) {
    perror("Failed to get the Landlock ABI version");
    return EXIT_FAILURE;
  }
  printf("Landlock version is %d\n", abi);

  // 今回はUbuntu 22.04,
  // kernel 6.2のABIバージョン3を前提としたプログラムなので、
  // それ以前のカーネル上では前方互換性のないフラグを無効化する。
  switch (abi) {
    case 1:  // ABI v1までは、
      // ディレクトリをまたがったrenameとハードリンクを許可できない
      access_fs_write &= ~LANDLOCK_ACCESS_FS_REFER;
    case 2:  // ABI v2までは、
      // truncateシステムコールを禁止できない
      access_fs_write &= ~LANDLOCK_ACCESS_FS_TRUNCATE;
  }

  // 許可しない限り拒否しておきたいオペレーションをすべて列挙する
  struct landlock_ruleset_attr ruleset_attr = {
      .handled_access_fs = access_fs_read | access_fs_write};
  // ルールセットを作成
  int ruleset_fd = syscall(__NR_landlock_create_ruleset, &ruleset_attr,
                           sizeof(ruleset_attr), 0);
  if (ruleset_fd < 0) {
    perror("Failed to create a ruleset");
    return EXIT_FAILURE;
  }

  // 一般的なコマンドの実行に必要なディレクトリと/homeのみに読み取りアクセスを許可
  const char *paths[] = {"/bin", "/usr", "/lib", "/home"};
  for (int i = 0; i < sizeof(paths) / sizeof(paths[0]); i++) {
    int fd = open(paths[i], O_PATH | O_CLOEXEC);
    if (fd < 0) {
      perror("open");
      return EXIT_FAILURE;
    }

    struct landlock_path_beneath_attr attr = {
        // 読み込みのみを許可
        .allowed_access = access_fs_read,
        // 対象のファイルパスを`O_PATH`で開いたFDを指定
        .parent_fd = fd,
    };
    if (syscall(__NR_landlock_add_rule, ruleset_fd,
                LANDLOCK_RULE_PATH_BENEATH, &attr, 0) < 0) {
      perror("landlock_add_rule");
      return EXIT_FAILURE;
    }
    close(fd);
  }

  // SUIDバイナリなどでの、root権限などへの昇格を無効化する
  // これを行わないと、次のlandlock_restrict_selfは失敗する
  if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
    perror("Failed to restrict privileges");
    return EXIT_FAILURE;
  }

  // Landlockを自分自身およびすべての子孫プロセスに適用
  if (syscall(__NR_landlock_restrict_self, ruleset_fd, 0) < 0) {
    perror("landlock_restrict_self");
    return EXIT_FAILURE;
  }
  // Bashへとexecする
  execl("/bin/bash", "bash", (char *)NULL);

  // execlは成功すればプログラムが切り替わるはずのシステムコールなので、
  // ここに到達した場合はシステムコールが失敗している
  perror("Failed to execl");
  return EXIT_FAILURE;
}
