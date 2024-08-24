static _Thread_local unsigned long static_main = 0;
extern _Thread_local unsigned long externed_hoge;

void initial_executable_access(void) { externed_hoge = 0xffffffffffffffff; }

void local_executable_access(void) { static_main = 0xdeadbeefdeadbeef; }

int main(void) { return 0; }
