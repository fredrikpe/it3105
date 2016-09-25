import random
import time


class GeneticAlgorithm:
    def __init__(self, initial_population, is_solution, fitness):
        self.fitness = fitness
        self.initial_population = initial_population
        self.is_solution = is_solution
        self.solutions = []

    def find_solutions(self, state):
        def sorting_key(state):
            return -self.fitness(state)
        t = 0

        solution_set = set()

        population = self.initial_population(state)
        print("initial pop: ", population)
        population.sort(key=sorting_key)

        board_size = len(population[0])

        while True:  #not self.stop_condition(t, board_size):

            parents = population[:self.parent_size(board_size)]
            #print("parents", parents)
            offsprings = self.crossover(parents, board_size)
            #print("offsprings", offsprings)
            offsprings = self.mutate(offsprings, board_size)
            #print("mutated", offsprings)

            tmp = population + offsprings

            tmp.sort(key=sorting_key)
            #print("tmp:\n", tmp)

            population = tmp[:self.population_size(board_size)]

            #print(population)

            for p in population:
                if self.is_solution(p):
                    #print("found solution", p)
                    solution_set.add(tuple(p))
                    self.solutions = [p]
                    return
                else:
                    break

            t += 1
        for s in solution_set:
            self.solutions.append(list(s))
        print(population)

    def crossover(self, parents, board_size):
        def make_offspring():
            father = random.choice(parents)
            mother = random.choice(parents)
            all_genes = set(range(board_size))
            offspring = [-1 for _ in range(board_size)]
            for i in range(board_size):
                if father[i] == mother[i] and father[i] not in offspring:
                    offspring[i] = father[i]
                    all_genes.remove(father[i])
            for i in range(board_size):
                if offspring[i] == -1:
                    offspring[i] = all_genes.pop()
            return offspring
        offsprings = parents[:self.offspring_size(board_size) // 2]
        offsprings.extend([make_offspring() for _ in range(self.offspring_size(board_size) // 2)])
        return offsprings
        #return [make_offspring(j) for j in range(self.offspring_size(board_size))]

    def mutate(self, offsprings, board_size):
        def mutation(queens):
            for _ in range(board_size // 4):
                i = random.randrange(board_size)
                j = (i + 1) % board_size
                queens[i], queens[j] = queens[j], queens[i]
            return queens

        return [mutation(o) for o in offsprings]

    @staticmethod
    def parent_size(board_size):
        return board_size

    @staticmethod
    def offspring_size(board_size):
        return board_size

    @staticmethod
    def population_size(board_size):
        return 2 * board_size

    def step_solve(self, state):
        return iter([])

    @staticmethod
    def stop_condition(t, board_size):
        return t > board_size * board_size * 2

