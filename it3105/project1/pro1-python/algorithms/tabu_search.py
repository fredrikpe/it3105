import time


class TabuSearch:
    def __init__(self, initial_solution, fitness, is_solution, neighborhood, max_tabu_size):
        self.initial_solution = initial_solution
        self.fitness = fitness
        self.is_solution = is_solution
        self.neighborhood = neighborhood
        self.max_tabu_size = max_tabu_size
        self.start_time = time.time()
        self.solutions = []
        self.initial_state = None
        self.tabu_list = []

    def step_solve(self, state):
        self.tabu_list = []
        yield from self.solver(state)

    def find_solutions(self, state):
        self.tabu_list = []
        board_size = len(self.initial_solution([0]))
        start = time.time()
        while not self.stop_condition(start, board_size):
            self.solver(state)

    def solver(self, state):
        steps = [state]

        current = self.initial_solution(state)
        current_best = current

        while True:

            best_candidate = None

            for neighbor in self.neighborhood(current):
                if neighbor not in self.tabu_list and self.fitness(neighbor) > self.fitness(best_candidate):
                    best_candidate = neighbor

            if not best_candidate:
                return steps

            current = best_candidate

            if self.fitness(best_candidate) > self.fitness(current_best):
                current_best = best_candidate
                steps.append(current_best)
                if self.is_solution(current_best):
                    if current_best not in self.solutions:
                        self.solutions.append(current_best)
                    return steps

            self.tabu_list.append(best_candidate)

            if len(self.tabu_list) > self.max_tabu_size:
                print(12323)
                self.tabu_list = self.tabu_list[1:]

        return steps

    def stop_condition(self, start, board_size):
        current_time = time.time()
        return current_time - start > board_size * board_size / 20



