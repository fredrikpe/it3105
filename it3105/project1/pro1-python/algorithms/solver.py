import copy
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

        self.genetic_algorithm = GeneticAlgorithm(self.initial_population, self.is_solution, self.fitness)

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
        L = len(queens)
        if L > self.board.board_size:
            return -1000

        # Horizontal conflicts (Number of duplicate elements).
        horizontal_conflicts = L - len(set(queens))

        diagonal_conflicts = L - len(set(queens[i] + i for i in range(L)))
        diagonal_conflicts += L - len(set(queens[i] - i for i in range(L)))



        # Diagonal conflicts (x and y distance is equal).
        # diagonal_conflicts = 0
        # for i in range(L):
        #     for j in range(i + 1, L):
        #
        #         y_dist = abs(queens[i] - queens[j])
        #         x_dist = abs(i - j)
        #
        #         if y_dist == x_dist:
        #             diagonal_conflicts += 1

        return - (diagonal_conflicts + 1.5 * horizontal_conflicts)

    def is_valid(self, queens):
        return self.fitness(queens) == 0

    def neighborhood_no_duplicates(self, queens):
        # Assumes no repetitions
        neighbors = []
        for i in range(self.board.board_size):
            def swap():
                permutations = []
                for j in range(i + 1, self.board.board_size):
                    new_state = [q for q in queens]
                    new_state[i], new_state[j] = new_state[j], new_state[i]
                    permutations.append(new_state)
                return permutations
            neighbors.extend(swap())
        return neighbors

    def neighborhood(self, queens):
        duplicates = self.board.board_size - len(set(queens))
        if duplicates == 0:
            return self.neighborhood_no_duplicates(queens)

        positions = [e for e in range(self.board.board_size) if e not in set(queens)]
        duplicates = [e for e in range(self.board.board_size) if e not in positions]

        neighbors = []

        for i in range(self.board.board_size):
            if queens[i] in duplicates:
                neighbor = copy.copy(queens)
                neighbor[i] = random.choice(positions)
                neighbors.append(neighbor)
        return neighbors

    def initial_solution(self, queens):
        if len(queens) < self.board.board_size:
            positions = set(range(self.board.board_size)) - set(queens)
            for i in range(len(queens), self.board.board_size):
                queens.append(positions.pop())
        return queens

    def initial_population(self, queens):
        if len(queens) < self.board.board_size:
            queens = self.initial_solution(queens)
        return self.neighborhood(queens)


    def reset_solutions(self):
        self.solutions = []
        self.backtracking.solutions = []
        self.tabu_search.solutions = []
        self.simulated_annealing.solutions = []
        self.genetic_algorithm.solutions = []