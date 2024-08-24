#include <errno.h>
#include <stdio.h>
#include <string.h>

// fuse.hをincludeする前に、FUSE_USER_VERSIONを定義
#define FUSE_USE_VERSION 31
#include <fuse.h>

#define MYFS_FILE_NAME "hello"
#define MYFS_MESSAGE "Hello, Binary Hacks!"

// ファイルシステムの初期化
static void *my_init(struct fuse_conn_info *conn,
                     struct fuse_config *cfg) {
  // 何もしない
  return NULL;
}

// getattr
// 与えられたpathにあるファイルやディレクトリの属性をstbufにセットする
static int my_getattr(const char *path, struct stat *stbuf,
                      struct fuse_file_info *fi) {
  int res = 0;

  memset(stbuf, 0, sizeof(struct stat));
  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
  } else if (strcmp(path + 1, MYFS_FILE_NAME) == 0) {
    // 読み込み専用ファイル
    stbuf->st_mode = S_IFREG | 0444;
    stbuf->st_nlink = 1;
    stbuf->st_size = sizeof(MYFS_MESSAGE);
  } else
    res = -ENOENT;

  return res;
}

// readdir
// 与えられたpathの指すディレクトリが含むエントリをbufにセット
static int my_readdir(const char *path, void *buf,
                      fuse_fill_dir_t filler, off_t offset,
                      struct fuse_file_info *fi,
                      enum fuse_readdir_flags flags) {
  // トップレベルのディレクトリ '/'以外にディレクトリは存在しない
  if (strcmp(path, "/") != 0)
    return -ENOENT;

  // ディレクトリの中に ".", "..", "hello"の3つが存在するように見せる
  filler(buf, ".", NULL, 0, 0);
  filler(buf, "..", NULL, 0, 0);
  filler(buf, MYFS_FILE_NAME, NULL, 0, 0);
  return 0;
}

// open
// 与えられたpathの指すファイルをopenしたときの処理
static int my_open(const char *path, struct fuse_file_info *fi) {
  // "hello"以外のファイルをopenしようとするとENOENT
  if (strcmp(path + 1, MYFS_FILE_NAME) != 0)
    return -ENOENT;

  if ((fi->flags & O_ACCMODE) != O_RDONLY)
    return -EACCES;

  return 0;
}

// read
// 与えられたpathの指すファイルの中身でbufを埋める
static int my_read(const char *path, char *buf, size_t size,
                   off_t offset, struct fuse_file_info *fi) {
  // "hello"以外のファイルを読もうとすると失敗する
  if (strcmp(path + 1, MYFS_FILE_NAME) != 0)
    return -ENOENT;

  // bufに"Hello, Binary Hacks"のメッセージを書き込む
  size_t len = sizeof(MYFS_MESSAGE);
  if (len <= offset) {
    return 0;
  }
  if (len < offset + size) {
    size = len - offset;
  }
  memcpy(buf, MYFS_MESSAGE + offset, size);

  return size;
}

static const struct fuse_operations my_fuse_ops = {
    .init = my_init,
    .getattr = my_getattr,
    .readdir = my_readdir,
    .open = my_open,
    .read = my_read,
};

int main(int argc, char *argv[]) {
  int ret;
  struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

  ret = fuse_main(args.argc, args.argv, &my_fuse_ops, NULL);
  fuse_opt_free_args(&args);

  return ret;
}
