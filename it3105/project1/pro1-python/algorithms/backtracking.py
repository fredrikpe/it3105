

class Backtracking:
    def __init__(self, is_valid, is_solution, generator):
        self.is_valid = is_valid
        self.is_solution = is_solution
        self.generator = generator
        self.solutions = []

    def step_solve(self, state):
        if not self.is_valid(state):
            return

        if self.is_solution(state):
            return

        for child in self.generator(state):
            yield child
            yield from self.solve(child)

    def solve(self, state):
        if self.is_valid(state):

            if self.is_solution(state):
                self.solutions.append(state)

            for child in self.generator(state):
                self.solve(child)

