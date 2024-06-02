def newline(f, repeat=1):
    for _ in range(repeat):
        f.write('\n')


def w(f, line: str, indent=0):
    f.write(' ' * indent + line)
    f.write('\n')


def write_if_else(f, comparisons, indent=0, else_str=None):
    idx = 0
    for lhs, rhs, body in comparisons:
        line = ''
        if idx > 0:
            line = '} else '
        line += f'if ({lhs} == {rhs}) {{'
        w(f, line, indent)
        w(f, body, indent + 2)
        idx += 1

    w(f, '}', indent)

    if else_str is not None:
        w(f, else_str, indent)
