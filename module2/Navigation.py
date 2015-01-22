
import sys
import math

class NavProblem:

    def __init__(self):

        self.mode, self.dim, self.start, self.goal, self.obstacles = self.get_input()

        self.valid_points = self.get_valid_points(self.dim, self.obstacles)

        if self.mode == "astar": self.i = 0
        elif self.mode == "dfs": self.i = 1
        elif self.mode == "bfs": self.i = 2
        else: print "Error: Unknown mode\n(Modes: astar, dfs, bfs)"; return

    def heur(self, (x1, y1), x2=None, y2=None):
        " Euclidian distance heuristic."
        if x2 is None:
            x2 = self.goal[0]
            y2 = self.goal[1]
        return math.sqrt((x1 - x2)**2 + (y1 - y2)**2)

    def succ(self, (x, y)):
        " Horizontal, vertical and diagonal moves allowed."
        predicate = lambda p:self.heur(p, x, y) < 1.5 and p != (x, y)
        return filter(predicate, self.valid_points)

    def is_goal(self, n):
        return n == self.goal

    def arc_cost(self, a, b):
        return self.heur(a, b[0], b[1])

    def get_valid_points(self, (x_dim, y_dim), obstacles):
        " Returns the points not obstructed."
        def obs_points((a, b, c, d)):
            return [(x, y) for x in range(a,a+c) for y in range(b,b+d)]
        all_obs_points = [p for o in obstacles for p in obs_points(o)]
        board = [(x, y) for x in range(x_dim) for y in range(y_dim)]
        return filter(lambda p: p not in all_obs_points, board)

    def get_input(self):
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
