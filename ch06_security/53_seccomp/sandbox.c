#include <fcntl.h>
#include <seccomp.h>
#include <stdio.h>
#include <unistd.h>

// 後で実装する
void untrusted_func(void);

int main(void) {
  // seccompによるフィルタリングを設定する
  // SCMP_ACT_KILLによって、許可していないシステムコールが実行された場合、
  // プログラムがkillされる
  scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_KILL);
  if (ctx == NULL) {
    perror("seccomp_init() failed");
    return 1;
  }

  // 実行してよいシステムコールのallow list
  // 今回は、"write"と"exit_group"の2つのシステムコールのみを許可する
  int allow_syscall_list[] = {
      SCMP_SYS(write),
      SCMP_SYS(exit_group),
  };

  for (int i = 0; i < sizeof(allow_syscall_list) / sizeof(int); i++) {
    if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, allow_syscall_list[i], 0) != 0) {
      perror("seccomp_rule_add() failed");
      return 1;
    }
  }

  if (seccomp_load(ctx) != 0) {
    perror("seccomp_load() failed");
    return 1;
  }

  untrusted_func();

  seccomp_release(ctx);
  return 0;
}
