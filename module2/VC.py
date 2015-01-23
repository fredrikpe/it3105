# -*- coding: utf-8 -*- #

import sys
import copy
import GAC


class VCProblem:

    def __init__(self):
        self.get_input()

        root = GAC.GACState(self.constraints, self.nodes)
        root.initialize()
        root.domain_filtering()
        self.start = root
        self.i = 0

    def succ(self, state):
        children = []
        for v in state.nodes:
            if len(v.domain) > 1:
                children = [copy.deepcopy(state) for i in range(len(v.domain))]
                for i in range(len(v.domain)):
                    children[i].nodes[v.index].domain = [v.domain[i]]
                    children[i].rerun(children[i].nodes[v.index])
                children =  filter(lambda c:not c.contra,children)
                assert(children != []) #No solution or bug
                return children

    def heur(self, state):
        return sum(len(v.domain)-1 for v in state.nodes)

    def is_goal(self, state):
        return all(map(lambda v:len(v.domain)==1, state.nodes))

    def arc_cost(self, state1, state2):
        return 1

    class Constraint:
        def __init__(self, vs, f):
            self.vars = vs
            self.f = f
    class Var:
        def __init__(self, index, x, y, K):
            self.p = (x, y)
            self.index = index
            self.domain = range(K)
        def __eq__(self, v):
            return self.index == v.index
        def __repr__(self):
            return str(self.p)

    def VC_f(self, n):
        return n[0] != n[1]

    def make_func(self, var_names, expression):
        args = ""
        for n in var_names:
            args += "," + n
        return eval("(lambda " + args[1:] + ": " + expression + ")")

    def make_var(self, l, s):
        var = self.Var(*(map(eval, s.split()) + [self.K]))
        l.append(var)
        return l

    def make_con(self, l, s):
        n, m = map(int, s.split())
        l.append(self.Constraint([self.nodes[n], self.nodes[m]], self.VC_f))
        return l

    def get_input(self):

        f = open(sys.argv[1], "r")
        ls = f.read().splitlines()
        nv, ne = map(int,ls[0].split())

        self.K = int(raw_input("K = "))

        self.nodes = reduce(self.make_var, ls[1:nv+1], [])
        self.constraints = reduce(self.make_con, ls[nv+1:], [])


def main():

    ns = VCProblem()

if __name__=='__main__':
    main()
