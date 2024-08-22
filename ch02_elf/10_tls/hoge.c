_Thread_local unsigned long externed_hoge = 0;
static _Thread_local unsigned long static_hoge = 0;

void general_dynamic_access(void) { externed_hoge = 0xaaaaaaaaaaaaaaaa; }

void local_dynamic_access(void) { static_hoge = 0xbbbbbbbbbbbbbbbb; }
