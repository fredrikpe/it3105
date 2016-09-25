import random
import time


class GeneticAlgorithm:
    def __init__(self, initial_population, is_solution, fitness, stop_condition):
        self.stop_condition = stop_condition
        self.fitness = fitness
        self.initial_population = initial_population
        self.is_solution = is_solution
        self.solutions = []

    def find_solutions(self, state):
        start_time = time.time()
        # def stop_condition():
        #     return self.stop_condition(start_time)
        self.solver(state, lambda: self.stop_condition(start_time))

    def step_solve(self, state):
        yield from self.solver(state)

    def solver(self, state, stop_condition=None):
        def step_stop_condition():
            return len(solution_set) != 0
        if stop_condition is None:
            stop_condition = step_stop_condition

        def sorting_key(state):
            return -self.fitness(state)

        steps = []
        solution_set = set()

        population = self.initial_population(state)
        population.sort(key=sorting_key)

        steps.append(population[0])

        board_size = len(population[0])

        while not stop_condition():

            parents = population[:self.parent_size(board_size)]

            offsprings = self.crossover_pmx(parents, board_size)

            offsprings = self.mutate(offsprings, board_size)

            tmp = population + offsprings

            tmp.sort(key=sorting_key)

            population = tmp[:self.population_size(board_size)]

            for p in population:
                if self.is_solution(p):
                    solution_set.add(tuple(p))
                else:
                    break

            steps.append(population[0])

        for s in solution_set:
            self.solutions.append(list(s))

        return steps

    def crossover_pmx(self, parents, board_size):
        def make_offspring():
            father = random.choice(parents)
            mother = random.choice(parents)

            i = random.randrange(board_size - 2)
            j = random.randrange(i + 1, board_size)

            brother = father[:i] + mother[i:j] + father[j:]
            sister = mother[:i] + father[i:j] + mother[j:]

            all_genes = set(range(board_size))

            for child in [brother, sister]:
                missing_genes = all_genes - set(child)
                if missing_genes:
                    for i in range(board_size):
                        if child[i] in child[i + 1:]:
                            child[i] = missing_genes.pop()

            return [brother, sister]
        offsprings = parents[:self.parent_size(board_size) // 2]
        for _ in range(self.parent_size(board_size) // 2):
            offsprings.extend(make_offspring())
        return offsprings

    def mutate(self, offsprings, board_size):
        def mutation(queens):
            for _ in range(1):
                i = random.randrange(board_size)
                j = (i + 1) % board_size
                queens[i], queens[j] = queens[j], queens[i]
            return queens

        return [mutation(o) for o in offsprings if random.random() > 0.2]

    @staticmethod
    def parent_size(board_size):
        return board_size // 2

    @staticmethod
    def offspring_size(board_size):
        return board_size

    @staticmethod
    def population_size(board_size):
        return 2 * board_size

    # @staticmethod
    # def stop_condition(t, board_size):
    #     return t > board_size * board_size * 2


        # def crossover(self, parents, board_size):
        #     def make_offspring():
        #         father = random.choice(parents)
        #         mother = random.choice(parents)
        #
        #         all_genes = list(range(board_size))
        #         random.shuffle(all_genes)
        #
        #         offspring = [-1 for _ in range(board_size)]
        #         for i in range(board_size):
        #             if father[i] == mother[i] and father[i] not in offspring:
        #                 offspring[i] = father[i]
        #                 all_genes.remove(father[i])
        #         for i in range(board_size):
        #             if offspring[i] == -1:
        #                 offspring[i] = all_genes.pop()
        #         return offspring
        #     offsprings = parents[:self.offspring_size(board_size) // 2]
        #     offsprings.extend([make_offspring() for _ in range(self.offspring_size(board_size) // 2)])
        #     return offsprings
        #     #return [make_offspring(j) for j in range(self.offspring_size(board_size))]