import random

import math

from algorithms.backtracking import Backtracking
from algorithms.genetic_algorithm import GeneticAlgorithm
from algorithms.tabu_search import TabuSearch
from algorithms.simulated_annealing import SimulatedAnnealing


class Solver:
    def __init__(self, board):
        self.board = board

        self.backtracking = Backtracking(self.board, self.is_solution, self.generator)

        self.tabu_search = TabuSearch(self.initial_solution, self.fitness, self.is_solution, self.neighborhood, 10000)

        self.simulated_annealing = SimulatedAnnealing(self.initial_solution, self.fitness, self.neighborhood, self.is_solution)

        self.genetic_algorithm = GeneticAlgorithm(self.initial_population, self.is_solution, self.fitness, self.crossover, self.mutate)

        self.selected_algorithm = self.backtracking
        self.input_length = 0

        self.solutions = set()

    def find_solutions(self, state):
        self.selected_algorithm.find_solutions(state)
        self.solutions = self.selected_algorithm.solutions

    def step_solve(self, state):
        return self.selected_algorithm.step_solve(state)

    def is_solution(self, queens):
        return self.is_valid(queens) and len(queens) == self.board.board_size

    def generator(self, queens):
        s = set(range(self.board.board_size))
        s.difference_update(queens)
        return [queens + [i] for i in s]

    def fitness(self, queens):
        if not queens:
            return -math.inf
        if len(queens) > self.board.board_size:
            return -1000

        # Diagonal conflicts (x and y distance is equal).
        diagonal_conflicts = 0
        for i in range(len(queens)):
            for j in range(i + 1, len(queens)):

                y_dist = abs(queens[i] - queens[j])
                x_dist = abs(i - j)

                if y_dist == x_dist:
                    diagonal_conflicts += 1

        return - diagonal_conflicts

    def is_valid(self, queens):
        return self.fitness(queens) == 0

    def neighborhood(self, queens):
        # Assumes no repetitions
        neighbors = []
        for i in range(self.input_length, self.board.board_size):
            def swap():
                permutations = []
                for j in range(i + 1, self.board.board_size):
                    new_state = [q for q in queens]
                    new_state[i], new_state[j] = new_state[j], new_state[i]
                    permutations.append(new_state)
                return permutations

            neighbors.extend(swap())

        return neighbors

    def initial_solution(self, queens):
        # Assumes already valid state, i.e. no repetitions
        positions = list(range(self.board.board_size))
        random.shuffle(positions)
        for i in positions:
            if i not in queens:
                queens.append(i)
        return queens

    def initial_population(self, queens):
        return [self.initial_solution(queens) for _ in range(self.board.board_size)]

    def crossover(self, parents):
        P = len(parents)
        def make_offspring():
            offspring = []
            for i in range(self.board.board_size):
                offspring.append(parents[random.randint(0, P - 1)][i])
            return offspring
        return [make_offspring() for i in range(self.board.board_size // 2)]

    def mutate(self, offsprings):
        positions = list(range(len(offsprings[0])))
        def mutation(queens):
            i = random.choice(positions)
            positions.remove(i)
            j = random.choice(positions)
            positions.append(i)
            queens[i], queens[j] = queens[j], queens[i]
            return queens
        return [mutation(o) for o in offsprings]

    def reset_solutions(self):
        self.solutions = []
        self.backtracking.solutions = []
        self.tabu_search.solutions = []
        self.simulated_annealing.solutions = []