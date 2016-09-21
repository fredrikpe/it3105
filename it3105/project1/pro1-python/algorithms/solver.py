import random

from algorithms.backtracking import Backtracking
from algorithms.tabu_search import TabuSearch
from algorithms.simulated_annealing import SimulatedAnnealing


class Solver:
    def __init__(self, board):
        self.board = board

        self.backtracking = Backtracking(self.is_valid, self.is_solution, self.generator)

        self.tabu_search = TabuSearch(self.initial_solution, self.fitness, self.is_solution, self.neighborhood, 100)

        self.simulated_annealing = SimulatedAnnealing(self.initial_solution, self.fitness, self.neighborhood, self.is_solution)

        self.selected_algorithm = self.backtracking
        self.input_length = 0

        self.solutions = set()

    def solve(self, state):
        self.selected_algorithm.solve(state)
        self.solutions = self.selected_algorithm.solutions

    def step_solve(self, state):
        return self.selected_algorithm.step_solve(state)

    def is_solution(self, queens):
        return self.is_valid(queens) and len(queens) == self.board.board_size

    def generator(self, queens):
        return [queens + [i] for i in range(self.board.board_size)]

    def fitness(self, queens):
        if not queens:
            return 0
        if len(queens) > self.board.board_size:
            return -1000
            # raise Exception("Queens list longer than board size.")

        # Horizontal conflicts (list contains duplicates).
        horizontal_conflicts = len(queens) - len(set(queens))

        # Diagonal conflicts (x and y distance is equal).
        diagonal_conflicts = 0
        for i in range(len(queens)):
            for j in range(i + 1, len(queens)):
                if i == j:
                    continue

                y_dist = abs(queens[i] - queens[j])
                x_dist = abs(i - j)

                if y_dist == x_dist:
                    diagonal_conflicts += 1

        return -(horizontal_conflicts + diagonal_conflicts)

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
                    tmp = new_state[i]
                    new_state[i] = new_state[j]
                    new_state[j] = tmp
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

    def reset_solutions(self):
        self.solutions = []
        self.backtracking.solutions = []
        self.tabu_search.solutions = []
        self.simulated_annealing.solutions = []