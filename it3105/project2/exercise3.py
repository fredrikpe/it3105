
import collections
import gym

from q_learning import QLearning

number_of_episodes = 6000
q_learning = QLearning(list(range(16)), [0, 1, 2, 3], number_of_episodes, learning_rate=0.7, start_eps=0.1)

env = gym.make('FrozenLake-v0')

hundred_slice = collections.deque()
hundred_counter = 1
average_over_slice = []

for i_episode in range(number_of_episodes):
    observation = env.reset()

    episode_reward = 0
    for t in range(1000):
        #env.render()
        #print(observation)

        action = q_learning.q_e_greedy(observation, i_episode)
        old_observation = observation
        observation, reward, done, info = env.step(action)

        q_learning.value_iteration_update_ex4(old_observation, action, reward, observation, i_episode)

        if done:
            episode_reward += reward
            #print("Episode finished after {} timesteps".format(t+1))
            break

    hundred_slice.append(episode_reward)
    if hundred_counter == 100:
        average_over_slice.append(sum(hundred_slice) / 100)
        hundred_slice.popleft()
    else:
        hundred_counter += 1

    #print("Episode:", i_episode, "Total reward:", episode_reward)

    # 0 Left
    # 1 Down
    # 2 Right
    # 3 Up

import matplotlib.pyplot as plt

plt.plot(average_over_slice)
plt.show()


print(q_learning.Q)