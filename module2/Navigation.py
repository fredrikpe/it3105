
import sys
import math


def heur((x1, y1), (x2, y2)):
    " Euclidian distance heuristic."
    return math.sqrt((x1 - x2)**2 + (y1 - y2)**2)

def succ((x, y), valid_points):
    " Horizontal, vertical and diagonal moves allowed."
    predicate = lambda p:heur(p, (x, y)) < 1.5 and p != (x, y)
    return filter(predicate, valid_points)

def get_valid_points((x_dim, y_dim), obstacles):
    " Returns the points not obstructed."
    def obs_points((a, b, c, d)):
        return [(x, y) for x in range(a,a+c) for y in range(b,b+d)]
    all_obs_points = [p for o in obstacles for p in obs_points(o)]
    board = [(x, y) for x in range(x_dim) for y in range(y_dim)]
    return filter(lambda p: p not in all_obs_points, board)

def get_input():
    g = lambda s: tuple(map(int, s.split(',')))
    obstacles = []
    if len(sys.argv) > 2:
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
    return sys.argv[2], dim, start, goal, obstacles
