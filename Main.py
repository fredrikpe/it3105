
import AStar
import itertools
import sys

# Manhattan heuristic
def man_heur((x1, y1), (x2, y2)):
    return abs(x1 - x2) + abs(y1 - y2)

def succ((x, y), valid_points):
    return filter(lambda p:man_heur(p, (x, y)) == 1, valid_points)

def get_valid_points((d1, d2), obstacles):
    g = lambda (a,b,c,d):[(x, y) for x in range(a,a+c) for y in range(b,b+d)]
    obs_points = [p for o in obstacles for p in g(o)]
    print obs_points
    return [(x, y) for x in range(d1) for y in range(d2) if (x, y) not in obs_points]

def main():
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

    valid_points = get_valid_points(dim, obstacles)

    solution = AStar.a_star(lambda p:succ(p, valid_points),
                            lambda p:man_heur(p, goal),
                            lambda p,q:1,
                            start,
                            lambda p:p==goal)
                            
    print solution

if __name__ == "__main__":
    main()
