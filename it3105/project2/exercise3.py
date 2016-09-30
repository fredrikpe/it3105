import random

import gym


class QLearning:
    def __init__(self, Q, action_space):
        self.Q = Q
        self.action_space = action_space

        self.e = 0.1
        self.discount = 0.99
        self.learning_rate = 0.3

    def value_iteration_update(self, state, action, reward, new_state):
        self.Q[state][action] += \
            self.learning_rate * (reward + self.discount * max(self.Q[new_state]) - self.Q[state][action])

    def Q_e_greedy(self, state):
        if random.random() > self.e:
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

q_learning = QLearning(myQ, [0, 1, 2, 3])

env = gym.make('FrozenLake-v0')

for i_episode in range(1000):
    observation = env.reset()

    for t in range(10000):
        episode_reward = 0
        #env.render()
        #print(observation)
        action = q_learning.Q_e_greedy(observation)
        old_observation = observation
        observation, reward, done, info = env.step(action)

        q_learning.value_iteration_update(old_observation, action, reward, observation)
        if reward > 0:
            print("reward", reward)
        episode_reward += reward

        if done:
            print("Episode finished after {} timesteps".format(t+1))
            break

    print("Episode:", i_episode, "Total reward:", episode_reward)

# 0 Left
# 1 Down
# 2 Right
# 3 Up