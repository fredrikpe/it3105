
import random

import math


class SimulatedAnnealing:
    def __init__(self, initial_state, fitness, neighborhood, is_solution):
        self.initial_state = initial_state
        self.fitness = fitness
        self.neighborhood = neighborhood
        self.is_solution = is_solution

        self.board_size = len(self.initial_state([1]))

    def solve(self, state):
        current_state = self.initial_state(state)

        for i, T in enumerate(self.temperatures()):

            neighbors = self.neighborhood(current_state)
            new_state = neighbors[random.randint(0, len(neighbors) - 1)]

            if self.probability(current_state, new_state, T) > random.uniform(0, 1):
                current_state = new_state
                print(self.fitness(current_state), i)

                yield current_state

            if self.is_solution(current_state):
                print(2)
                yield current_state

        print("Outside loop", i)
        yield current_state

    def probability(self, current_state, new_state, T):
        difference = self.fitness(current_state) - self.fitness(new_state)
        if difference < 0:
            return 1
        return math.exp(-difference/T)

    def temperatures(self):
        k = 0
        while k < 1000:
            yield 2 * len(self.initial_state([0])) * pow(0.95, k)
            k += 1




