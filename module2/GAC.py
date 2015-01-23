
class GACState:

    def __init__(self, constraints, nodes):
        self.constraints = constraints
        self.nodes = nodes
        self.queue = []
        self.contra = False

    def initialize(self):
        self.queue = [[c, var] for c in self.constraints for var in c.vars]

    def domain_filtering(self):
        while self.queue:
            C, x = self.queue.pop()
            old = len(x.domain)
            self.revise(C, x)
            new = len(x.domain)
            if old > new:
                self.push_pairs(x, C)

    def revise(self, C, x):
        i = x.index
        var_combs = map(list,zip(*[var.domain for var in C.vars if var != x]))
        pred = lambda x: any(map(lambda vs: C.f(vs[:i]+[x]+vs[i:]), var_combs))
        x.domain = filter(pred, x.domain)
        if x.domain == []:
            self.contra = True

    def rerun(self, x):
        self.push_pairs(x)
        self.domain_filtering()

    def push_pairs(self, x, C=None):
        for c in self.constraints:
            if x in c.vars and (C is None or c != C):
                for var in c.vars:
                    if var != x:
                        self.queue.append([c, var])

    def __repr__(self):
        s = ""
        for n in self.nodes:
            s += str(n.domain) + ", "
        return s
