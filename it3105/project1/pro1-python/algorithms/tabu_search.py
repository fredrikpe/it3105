import time


class TabuSearch:
    def __init__(self, initial_solution, fitness, is_solution, neighborhood, max_tabu_size):
        self.initial_solution = initial_solution
        self.fitness = fitness
        self.is_solution = is_solution
        self.neighborhood = neighborhood
        self.max_tabu_size = max_tabu_size
        self.start_time = time.time()

    def solve(self, state):
        self.start_time = time.time()

        current = self.initial_solution(state)
        current_best = current
        tabu_list = []

        while not self.stop_condition():

            best_candidate = None

            for neighbor in self.neighborhood(current):

                if neighbor not in tabu_list and self.fitness(neighbor) > self.fitness(best_candidate):
                    best_candidate = neighbor

            current = best_candidate

            if self.fitness(best_candidate) > self.fitness(current_best):
                current_best = best_candidate
                yield current_best

            tabu_list.append(best_candidate)

            if len(tabu_list) > self.max_tabu_size:
                tabu_list = tabu_list[1:]

        yield current_best

    def stop_condition(self):
        current_time = time.time()
        # 5 seconds
        #print(current_time - self.start_time > 10)
        return current_time - self.start_time > 10


