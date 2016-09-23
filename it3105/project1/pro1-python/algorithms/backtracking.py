

class Backtracking:
    def __init__(self, board, is_solution, generator):
        self.board = board
        self.is_solution = is_solution
        self.generator = generator
        self.solutions = []

    def step_solve(self, state):
        if self.is_valid(state):

            if self.is_solution(state):
                return

            for child in self.generator(state):
                yield child
                yield from self.step_solve(child)

    def find_solutions(self, state):
        if self.is_valid(state):

            if self.is_solution(state):
                self.solutions.append(state)
                return

            for child in self.generator(state):
                self.find_solutions(child)

    @staticmethod
    def is_valid(queens):
        L = len(queens)
        if L > 1:

            # Diagonal conflicts (x and y distance is equal).
            y_pos = queens[-1]
            for i in range(L - 1):

                if abs(queens[i] - y_pos) == L - 1 - i:
                    return False

        return True
