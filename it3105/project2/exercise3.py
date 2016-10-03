import random
import numpy
import collections

import gym


class QLearning:
    def __init__(self, Q, action_space):
        self.Q = Q
        self.action_space = action_space

        self.discount = 0.99
        self.learning_rate = 0.8
        self.epsilon_list = self.epsilon()

    @staticmethod
    def epsilon():
        return numpy.linspace(0.1, 0, number_of_episodes)

    def value_iteration_update(self, state, action, reward, new_state):
        self.Q[state][action] += \
            self.learning_rate * (reward + self.discount * max(self.Q[new_state]) - self.Q[state][action])

    def value_iteration_update_4(self, state, action, reward, new_state, episode_number):
        self.Q[state][action] += \
            self.learning_rate * (reward + self.discount * self.Q[new_state][self.Q_e_greedy(state, episode_number)] - self.Q[state][action])

    def Q_e_greedy(self, state, episode_number):
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



myQ = []
for s in range(16):
    myQ.append([0 for i in range(4)])

number_of_episodes = 3000
q_learning = QLearning(myQ, [0, 1, 2, 3])

env = gym.make('FrozenLake-v0')

hundred_slice = collections.deque()
hundred_counter = 1

for i_episode in range(number_of_episodes):
    observation = env.reset()


    episode_reward = 0
    for t in range(10000):
        #env.render()
        #print(observation)
        action = q_learning.Q_e_greedy(observation, i_episode)
        old_observation = observation
        observation, reward, done, info = env.step(action)

        q_learning.value_iteration_update_4(old_observation, action, reward, observation, i_episode)

        if done:
            episode_reward += reward
            #print("Episode finished after {} timesteps".format(t+1))
            break

    hundred_slice.append(episode_reward)
    if hundred_counter == 100:
        print(sum(hundred_slice))
        hundred_slice.popleft()
    else:
        hundred_counter += 1

    #print("Episode:", i_episode, "Total reward:", episode_reward)

    # 0 Left
    # 1 Down
    # 2 Right
    # 3 Up