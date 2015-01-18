
import itertools
import sys
import math

# Manhattan heuristic
def heur((x1, y1), (x2, y2)):
    return math.sqrt((x1 - x2)**2 + (y1 - y2)**2)

def succ((x, y), valid_points):
    return filter(lambda p:heur(p, (x, y)) <= 1.5, valid_points)

def get_valid_points((d1, d2), obstacles):
    g = lambda (a,b,c,d):[(x, y) for x in range(a,a+c) for y in range(b,b+d)]
    obs_points = [p for o in obstacles for p in g(o)]
    return [(x, y) for x in range(d1) for y in range(d2) if (x, y) not in obs_points]

def get_input():
    g = lambda s: tuple(map(int, s.split(',')))
    obstacles = []
    if len(sys.argv) > 1:
        f = open(sys.argv[1], "r")
        ls = f.read().splitlines()
        dim, start, goal = g(ls[0]), g(ls[1]), g(ls[2])
        ls = ls[3:]
        while ls:
            obstacles.append(list(g(ls[0])))
            ls = ls[1:]
    else:
        h = lambda s:g(raw_input(s).split(','))
        dim, start, goal = h("Dim: "), h("Start: "), h("Goal: ")
        obstacles = []
        obs = raw_input("Obstacle: ")
        while obs != "":
            obstacles.append(map(int, obs.split(',')))
            obs = raw_input("Obstacle: ")

    return dim, start, goal, obstacles

if __name__ == "__main__":
    main()
