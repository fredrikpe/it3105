# -*- coding: utf-8 -*- #

class Constraint:
    def __init__(self, vs, f):
        self.vars = vs
        self.f = f

class Var:
    def __init__(self, colors):
        self.domain = range(colors)

def make_func(var_names, expression):
    args = ""
    for n in var_names:
        args += "," + n
    return eval("(lambda " + args[1:] + ": " + expression + ")")

#def get_input():


def main():
    f= make_func("xyz", "z+y == x")
    print f(1,2,3)

if __name__=='__main__':
    main()
