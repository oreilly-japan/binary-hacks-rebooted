#include <asm/kvm.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/kvm.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#define MEM_SIZE 1024000

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <binary file>\n", argv[0]);
    return 1;
  }

  char *binary = argv[1];
  int dev = open("/dev/kvm", O_RDWR);
  if (dev < 0) {
    perror("open failed");
    return 1;
  }

  // KVM API versionの取得
  long kvm_api_version = ioctl(dev, KVM_GET_API_VERSION);
  printf("KVM API version: %ld\n", kvm_api_version);

  // VMの作成
  int vm_fd = ioctl(dev, KVM_CREATE_VM);
  if (vm_fd < 0) {
    perror("KVM_CREATE_VM failed");
    return 1;
  }

  // メモリの割り当て
  void *load_addr =
      mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE,
           MAP_ANONYMOUS | MAP_SHARED | MAP_NORESERVE, -1, 0);
  if (load_addr == MAP_FAILED) {
    perror("mmap failed");
    return 1;
  }
  struct kvm_userspace_memory_region mem = {
      .slot = 0,
      .flags = 0,
      .guest_phys_addr = 0,
      .memory_size = MEM_SIZE,
      .userspace_addr = (uint64_t)load_addr,
  };
  int ret = ioctl(vm_fd, KVM_SET_USER_MEMORY_REGION, &mem);
  if (ret < 0) {
    perror("KVM_SET_USER_MEMORY_REGION failed");
    return 1;
  }

  // バイナリをメモリにロード
  FILE *file = fopen(binary, "rb");
  if (!file) {
    perror("fopen failed");
    return 1;
  }
  fread(load_addr, MEM_SIZE, 1, file);
  fclose(file);

  // vCPUの作成
  int vcpu_fd = ioctl(vm_fd, KVM_CREATE_VCPU, 0);
  if (vcpu_fd < 0) {
    perror("KVM_CREATE_VCPU failed");
    return 1;
  }

  // vCPUのメモリマップ
  int kvm_run_mmap_size = ioctl(dev, KVM_GET_VCPU_MMAP_SIZE, NULL);
  if (kvm_run_mmap_size < 0) {
    perror("KVM_GET_VCPU_MMAP_SIZE failed");
    return 1;
  }
  void *kvm_run_ptr =
      mmap(NULL, kvm_run_mmap_size, PROT_READ | PROT_WRITE,
           MAP_SHARED, vcpu_fd, 0);
  if (kvm_run_ptr == MAP_FAILED) {
    perror("mmap failed");
    return 1;
  }

  // 汎用レジスタの初期化
  struct kvm_regs regs;
  ret = ioctl(vcpu_fd, KVM_GET_REGS, &regs);
  if (ret < 0) {
    perror("KVM_GET_REGS failed");
    return 1;
  }
  // rflagsの1bit目は常に1。それ以外のbitをリセット
  regs.rflags = 1 << 1;
  regs.rip = 0;
  regs.rsp = 0xffffffff;
  regs.rbp = 0;
  ret = ioctl(vcpu_fd, KVM_SET_REGS, &regs);
  if (ret < 0) {
    perror("KVM_SET_REGS failed");
    return 1;
  }

  // セグメントレジスタの初期化
  struct kvm_sregs sregs;
  ret = ioctl(vcpu_fd, KVM_GET_SREGS, &sregs);
  if (ret < 0) {
    perror("KVM_GET_SREGS failed");
    return 1;
  }
  sregs.cs.selector = 0;
  sregs.cs.base = 0;
  ret = ioctl(vcpu_fd, KVM_SET_SREGS, &sregs);
  if (ret < 0) {
    perror("KVM_SET_SREGS failed");
    return 1;
  }

  // VMの実行
  while (1) {
    int ret = ioctl(vcpu_fd, KVM_RUN, 0);
    if (ret < 0) {
      perror("KVM_RUN failed");
      return 1;
    }

    // KVM_RUNが返ってきたら、KVM_EXIT_REASONに応じて処理を行う
    struct kvm_run *kvm_run = (struct kvm_run *)kvm_run_ptr;
    int exit_reason = kvm_run->exit_reason;
    switch (exit_reason) {
    case KVM_EXIT_HLT:
      puts("KVM_EXIT_HLT");
      return 0;
    case KVM_EXIT_IO:
      if (kvm_run->io.direction == KVM_EXIT_IO_IN) {
        // シリアルポートからの入力待ち
        printf("Enter a number: ");
        char *in_data = (char *)kvm_run_ptr;
        int offset = kvm_run->io.data_offset;
        scanf("%d", (int *)(in_data + offset));
      } else {
        // シリアルポートへの出力
        unsigned char *out_data = (unsigned char *)kvm_run_ptr;
        int port = kvm_run->io.port;
        int offset = kvm_run->io.data_offset;
        printf("[%d]: %u\n", port, out_data[offset]);
      }
      break;
    case KVM_EXIT_INTERNAL_ERROR:
      puts("KVM_EXIT_INTERNAL_ERROR");
      return 1;
    default:
      fprintf(stderr, "Unsupported exit reason: %d", exit_reason);
      return 1;
    }
  }

  return 0;
}
