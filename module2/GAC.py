
class GACState:

    def __init__(self, constraints):
        self.constraints = constraints

    def initialize(self):
        queue = [[c, var] for c in self.constraints for var in c.vars]

    def domain_filtering(self):
        while queue:
            C, x = queue.pop()
            old = len(x.domain)
            revise(C, x)
            new = len(x.domain)
            if old > new:
                self.push_pairs(x, C)

    def rerun(self, X):
        "(Given that the assumption X* = x was just made)"
        self.push_pairs(X)
        self.domain_filtering()

    def revise(self, C, x):
        i = str(C.vars.index(x))
        var_combs = zip(*[var.domain for var in C.vars if var != x])
        pred = lambda x: any(map(lambda vs: C.f(vs[:i].append(x)+vs[i:])), var_combs))
        x.domain = filter(pred, x.domain)

    def push_pairs(self, x, C=None):
        for c in self.constraints:
            if x in c.vars and (C is None or c != C):
                for var in c.vars:
                    if var != x:
                        queue.append([c, var])




Assume constraint C consists of variable X plus variables Y1, Y2, ..., Yn,
and assume that REVISE*(X,C) is called,
meaning that X is the focal variable to be tested against the other variables in constraint C.

Retain all and only those x in Domain(X) where there exists (y0, y1, ... , yn)
with yi in Domain(Yi) forall i,
and(x, y0, y1, ... , yn) satisfies C
