
class GACState:

    def __init__(self, nodes):

        self.nodes = nodes
        self.queue = []
        self.contra = False

        assert(self.constraints is not None)

    def initialize(self):
        self.queue = [[c, i] for c in self.constraints for i in c.vars]

    def domain_filtering(self):
        while self.queue and not self.contra:
            C, i = self.queue.pop()
            old = len(self.nodes[i])
            self.revise(C, i)
            new = len(self.nodes[i])
            if old > new:
                self.push_pairs(i, C)

    def revise(self, C, i):
        var_combs = map(list,zip(*[self.nodes[j] for j in C.vars if j != i]))
        def predicate(x):
            return any(map(lambda vs: C.f(vs+[x]), var_combs))
        self.nodes[i] = filter(predicate, self.nodes[i])
        if self.nodes[i] == []:
            self.contra = True

    def rerun(self, i):
        self.push_pairs(i)
        self.domain_filtering()

    def push_pairs(self, i, C=None):
        """ Finds (constraint, variable) pair affected by assumption
        and pushes pair to queue.
        """
        for c in self.constraints:
            if i in c.vars and (C is None or c != C):
                for j in c.vars:
                    if j != i:
                        self.queue.append([c, j])
