
import random

import math

import time


class SimulatedAnnealing:
    def __init__(self, initial_solution, fitness, neighborhood, is_solution, stop_condition):
        self.stop_condition = stop_condition
        self.initial_solution = initial_solution
        self.fitness = fitness
        self.neighborhood = neighborhood
        self.is_solution = is_solution

        self.board_size = len(self.initial_solution([1]))

        self.solutions = []

    def solver(self, state, board_size):
        current_state = self.initial_solution(state)
        steps = []

        for i, T in enumerate(self.temperatures(board_size)):

            neighbors = self.neighborhood(current_state)
            new_state = neighbors[random.randint(0, len(neighbors) - 1)]

            if self.probability(current_state, new_state, T) > random.uniform(0, 1):
                current_state = new_state
                steps.append(current_state)

            if self.is_solution(current_state):
                if current_state not in self.solutions:
                    self.solutions.append(current_state)
                return steps

        return steps

    def step_solve(self, state):
        board_size = len(self.initial_solution([0]))
        yield from self.solver(state, board_size)

    def find_solutions(self, state):
        start_time = time.time()
        board_size = len(self.initial_solution([0]))

        while not self.stop_condition(start_time):
            self.solver(state, board_size)

    def find_one_solution(self, state):
        board_size = len(self.initial_solution([0]))
        while not self.solutions:
            self.solver(state, board_size)

    def probability(self, current_state, new_state, T):
        difference = self.fitness(current_state) - self.fitness(new_state)
        if difference < 0:
            return 1
        return math.exp(-difference / T)

    def temperatures(self, board_size):
        k = 0
        while k < board_size * board_size:
            yield 1 * pow(0.95, k)
            k += 1

    # def stop_condition(self, board_size):
    #     current_time = time.time()
    #     return current_time - self.start_time > board_size * board_size / 20



