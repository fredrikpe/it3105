import time


class GeneticAlgorithm:
    def __init__(self, initial_population, is_solution, fitness, crossover, mutate):
        self.fitness = fitness
        self.initial_population = initial_population
        self.is_solution = is_solution
        self.crossover = crossover
        self.mutate = mutate
        self.solutions = []

    def find_solutions(self, state):
        def sorting_key(state):
            return self.fitness(state)
        t = 0

        population = self.initial_population(state)
        population.sort(key=sorting_key)

        board_size = len(population[0])

        while not self.stop_condition(t):

            print("population\n", population)

            parents = population[:board_size // 2]

            offsprings = self.crossover(parents)
            print("offsprings\n", offsprings)
            offsprings = self.mutate(offsprings)
            print("mutated\n", offsprings)

#
            # 1 3 2 0
            # 3 2 0 1
            #
            # 1 3
            tmp = population + offsprings

            tmp.sort(key=sorting_key)
            print("tmp:\n", tmp)

            population = tmp[:board_size]

            for p in population:
                if self.is_solution(p):
                    print("found solution")
                    if p not in self.solutions:
                        self.solutions.append(p)
                else:
                    break

            t += 1

    def step_solve(self, state):
        return iter([])

    def stop_condition(self, t):
        return t > 5

