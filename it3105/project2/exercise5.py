
import collections
import gym

from q_learning import QLearning

number_of_episodes = 3000
q_learning = QLearning(list(range(16)), [0, 1, 2, 3], number_of_episodes)

q_function_from_ex3 = [[0.6404337209307732, 0.4682231075937863, 0.4079467392829239, 0.2580601469256166],
                       [0.2021507732124564, 0.01370148182268461, 0.05453085232697666, 0.6490903047892156],
                       [0.16839175015955846, 0.128140760911408, 0.10434377387568707, 0.6821175013479859],
                       [0.06792122773228362, 0.0658009142330244, 0.019490267012074416, 0.4556433607840507],
                       [0.6175141385546384, 0.3735085673972577, 0.16605311989482713, 0.05549046842730404],
                       [0, 0, 0, 0],
                       [0.5138414897520849, 0.0012722651028569463, 0.002309863973251577, 4.3286416582008356e-05],
                       [0, 0, 0, 0],
                       [0.12030990228679578, 0.14896527464954223, 0.15891422357051427, 0.7560014967094633],
                       [0.04925551451178141, 0.7479462515758075, 0.059582820481261234, 0.06549452275014503],
                       [0.8299517442739417, 0.018306415441519536, 0.023269158010844518, 0.00879629382596745],
                       [0, 0, 0, 0],
                       [0, 0, 0, 0],
                       [0.0058237218521948846, 0.2841949255087657, 0.8757723079491124, 0.015291555412039845],
                       [0.3151641827038803, 0.9586565395949352, 0.3787965321606538, 0.45716484019446324],
                       [0, 0, 0, 0]]

env = gym.make('FrozenLake-v0')

hundred_slice = collections.deque()
hundred_counter = 1

for i_episode in range(number_of_episodes):
    observation = env.reset()


    episode_reward = 0
    for t in range(10000):
        #env.render()
        #print(observation)

        action = q_learning.q_e_greedy(observation, i_episode)
        old_observation = observation
        observation, reward, done, info = env.step(action)

        q_learning.value_iteration_update_ex5(old_observation, action, reward, observation, q_function_from_ex3)

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






