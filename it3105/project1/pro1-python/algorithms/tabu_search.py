import time


class TabuSearch:
    def __init__(self, initial_solution, fitness, is_solution, neighbourhood, stop_condition):
        self.stop_condition = stop_condition
        self.initial_solution = initial_solution
        self.fitness = fitness
        self.is_solution = is_solution
        self.neighbourhood = neighbourhood
        self.max_tabu_size = 10000
        self.start_time = time.time()
        self.solutions = []
        self.initial_state = None
        self.tabu_list = []

    def step_solve(self, state):
        self.tabu_list = []
        yield from self.solver(state)

    def find_solutions(self, state):
        self.tabu_list = []

        start_time = time.time()
        while not self.stop_condition(start_time):
            self.solver(state)

    def solver(self, state):
        steps = [state]

        current = self.initial_solution(state)
        current_best = current

        while True:

            best_candidate = None

            for neighbour in self.neighbourhood(current):
                if neighbour not in self.tabu_list and self.fitness(neighbour) > self.fitness(best_candidate):
                    best_candidate = neighbour

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
                self.tabu_list = self.tabu_list[1:]

        return steps




