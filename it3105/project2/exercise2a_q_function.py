import random
import gym


def Q_soft_max():
    choice = random.randint(0, 4)
    return [1, 1, 0, 2, 3][choice]


def Q_e_greedy(e):
    if random.uniform > e:
        return 1
    return random.choice([0,2,3])


def Q_greedy():
    return 1


env = gym.make('FrozenLake-v0')

for i_episode in range(20):
    observation = env.reset()

    for t in range(100):
        env.render()
        print(observation)
        action = Q_soft_max()
        observation, reward, done, info = env.step(action)
        if done:
            print("Episode finished after {} timesteps".format(t+1))
            break

# 0 Left
# 1 Down
# 2 Right
# 3 Up