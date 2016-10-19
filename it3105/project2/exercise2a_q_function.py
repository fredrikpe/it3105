import random
import gym

Q = [0.5, 1, 0.5, 0.5]


def Q_e_greedy(e):
    greedy = Q_greedy()
    if random.uniform > e:
        return greedy
    return random.choice(list(range(len(Q))).remove(greedy))


def Q_greedy():
    return Q.index(max(Q))


env = gym.make('FrozenLake-v0')

for i_episode in range(20):
    observation = env.reset()

    for t in range(100):
        env.render()
        print(observation)
        action = Q_e_greedy()
        observation, reward, done, info = env.step(action)
        if done:
            print("Episode finished after {} timesteps".format(t+1))
            break

# 0 Left
# 1 Down
# 2 Right
# 3 Up