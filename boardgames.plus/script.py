
BOARD_WIDTH = 7
BOARD_HEIGHT = 6

# Rows
for i in range(BOARD_HEIGHT):
    s = '{'
    for j in range(BOARD_WIDTH):
        s += f'{{{i}, {j}}}, '
    print(s + '}')

# Rows
for j in range(BOARD_WIDTH):
    s = '{'
    for i in range(BOARD_HEIGHT):
        s += f'{{{i}, {j}}}, '
    print(s + '}')

# Diags -1 +1
for i in range(BOARD_HEIGHT - 1, -1, -1):
    s = '{'
    for d in range(i + 1):
        s += f'{{{i - d}, {d}}}, '
    print(s + '}')

for j in range(1, BOARD_WIDTH):
    s = '{'
    for d in range(BOARD_WIDTH - j):
        s += f'{{{BOARD_HEIGHT - 1 - d}, {j + d}}}, '
    print(s + '}')

# Diags +1 +1
for i in range(BOARD_HEIGHT):
    s = '{'
    for d in range(BOARD_HEIGHT - i):
        s += f'{{{i + d}, {d}}}, '
    print(s + '}')

for j in range(1, BOARD_WIDTH):
    s = '{'
    for d in range(BOARD_WIDTH - j):
        s += f'{{{d}, {j + d}}}, '
    print(s + '}')



