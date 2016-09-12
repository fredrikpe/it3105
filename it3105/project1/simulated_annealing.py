

import random

class SimulatedAnnealing:
    def __init__(self, initial_state, neighborhood, probability, temperatures):
        self.initial_state = initial_state
        self.neighborhood = neighborhood
        self.probability = probability
        self.temperatures = temperatures

    def solve(state):
        current_state = self.initial_state(state)

        for T in self.temperatures:

            neighbors = self.neighborhood(current_state)
            new_state = neighbors[random.randint(0, len(neighbors))]

            if self.probability(current_state, new_state, T) > random.random(0, 1):
                current_state = new_state

            if is_solution(current_state):
                yield current_state

        yield current_state



