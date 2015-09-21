# -*- coding: utf-8 -*- #

import sys
import copy
import GAC

import itertools

class VCProblem:

    def __init__(self):
        self.points = {}
        self.nodes = {}
        self.constraints = []
        self.get_input()

        self.Constraint.f = self.VC_f

        GAC.GACState.constraints = self.constraints
        root = GAC.GACState(self.nodes)
        root.initialize()
        root.domain_filtering()
        self.start = root
        self.i = 0

    def succ(self, state):
        " Generates children and runs the rerun method. "
        children = []
        for i, d in state.nodes.iteritems():
            if len(d) > 1:
                children = []
                for j in range(len(d)):
                    child = copy.deepcopy(state)
                    child.nodes[i] = [d[j]]
                    child.rerun(i)
                    if not child.contra:
                        children.append(child)
                assert(children != []) #No solution or bug
                return children

    def heur(self, state):
        return sum(len(d)-1 for d in state.nodes.itervalues())

    def is_goal(self, state):
        return all(map(lambda d:len(d)==1, state.nodes.itervalues()))

    def arc_cost(self, state1, state2):
        return 1

    class Constraint:
        def __init__(self, vs):
            self.vars = vs

    def VC_f(self, n):
        return n[0] != n[1]

    def make_func(self, var_names, expression):
        args = ""
        for n in var_names:
            args += "," + n
        return eval("(lambda " + args[1:] + ": " + expression + ")")

    def make_var(self, l, s):
        " Create variable and adds to list. "
        index, x, y = map(eval, s.split())
        self.points[index] = (x, y)
        self.nodes[index] = range(self.K)
        return l

    def make_con(self, l, s):
        " Create constraint and add to list. "
        n, m = map(int, s.split())
        l.append(self.Constraint([n,m]))
        return l

    def get_input(self):
        " From file and asks K from user. "
        f = open(sys.argv[1], "r")
        ls = f.read().splitlines()
        nv, ne = map(int,ls[0].split())

        self.K = int(raw_input("K = "))

        n_list = reduce(self.make_var, ls[1:nv+1], [])
        self.constraints = reduce(self.make_con, ls[nv+1:], [])



def main():

    ns = VCProblem()

if __name__=='__main__':
    main()
