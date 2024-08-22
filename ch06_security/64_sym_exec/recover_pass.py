import angr
import claripy  # angr の制約ソルバー

LEN_PASSWORD = 14

# `puts("You did it!")` のバイナリ内でのアドレス
TARGET_ADDR = 0x4012BF
# `puts("Fail :(")` のバイナリ内でのアドレス
FAIL_ADDR = 0x401295

proj = angr.Project("./check_password")

# コマンドライン引数のシンボル
# 挙動を安定させるために長めに用意する
# 動かすマシンのスペックに合わせて適宜短くする
argv1 = claripy.BVS("argv1", 8 * 1024)

# argv[1] をシンボルにして初期状態を作成
state = proj.factory.entry_state(args=["./check_password", argv1])

# argv1 が表示可能な文字で構成されていることを制約にする
for i in range(LEN_PASSWORD):
    char = argv1.get_byte(i)
    state.add_constraints(0x21 <= char, char <= 0x7E)
state.add_constraints(argv1.get_byte(LEN_PASSWORD) == 0)

# シンボリック実行の状態管理マネージャーを生成
sm = proj.factory.simulation_manager(state)

# 状態を 1 命令ずつ進める。`puts("You did it")` が実行されたら終了
while len(sm.active) > 0:
    print(sm)
    sm.explore(find=TARGET_ADDR, avoid=FAIL_ADDR, n=1)

    # 探索途中の argv[1] を表示
    for state in sm.active:
        print(state.solver.eval(argv1, cast_to=bytes)[: LEN_PASSWORD + 1])

    if len(sm.found) > 0:
        break

state = sm.found[0]
print(
    "Password is cracked!!:",
    state.solver.eval(argv1, cast_to=bytes)[: LEN_PASSWORD + 1],
)
