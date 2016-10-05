import random
import numpy


class QLearning:
    def __init__(self, state_space, action_space, number_of_episodes):
        self.state_space = state_space
        self.action_space = action_space
        self.number_of_episodes = number_of_episodes

        self.Q = []
        for _ in state_space:
            self.Q.append([0 for _ in action_space])

        self.discount = 0.99
        self.learning_rate = 0.7
        self.epsilon_list = self.epsilon()

    def epsilon(self):
        return numpy.linspace(0.1, 0, self.number_of_episodes)

    def value_iteration_update_ex3(self, state, action, reward, new_state):
        self.Q[state][action] += \
            self.learning_rate * (reward + self.discount * max(self.Q[new_state]) - self.Q[state][action])

    def value_iteration_update_ex4(self, state, action, reward, new_state, episode_number):
        self.Q[state][action] += \
            self.learning_rate * (reward + self.discount * self.Q[new_state][self.q_e_greedy(state, episode_number)] -
                                  self.Q[state][action])

    def value_iteration_update_ex5(self, state, action, reward, new_state, q_function):
        self.Q[state][action] += \
            self.learning_rate * (reward + self.discount * max(q_function[new_state]) - self.Q[state][action])

    def q_e_greedy(self, state, episode_number):
        eps = self.epsilon_list[episode_number]
        if random.random() > eps:
            max_value = -1000
            max_indexes = []
            for i, e in enumerate(self.Q[state]):
                if e > max_value:
                    max_value = e
                    max_indexes = [i]
                elif e == max_value:
                    max_indexes.append(i)

            w = random.choice(max_indexes)
            return w
        w = random.randint(0, 3)
        return w
