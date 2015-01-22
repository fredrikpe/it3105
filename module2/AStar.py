
import heapq as hq

class AStar:
    
    def __init__(self, problem_instance):
        self.P = problem_instance
        self.agenda, self.closed, self.paths = [], [], []
        self.add((self.P.heur(self.P.start), self.P.start))
        self.g = {self.P.start:0}
        self.came_from = {}

    def solve(self):
        " A general algorithm for A*, dfs and bfs search. "
        P, g, = self.P, self.g
        while self.agenda:

            parent, self.agenda = self.pop()
            self.closed.append(parent)
            self.paths.append(self.reconstruct_path([parent]))

            if P.is_goal(parent):
                return self.paths

            for child in P.succ(parent):
                new_g = g[parent] + P.arc_cost(parent, child)
                if child in self.closed:
                    continue

                c_in_a = self.contains(child)
                if not c_in_a or (child in g and new_g < g[child]):
                    g[child] = new_g
                    f = new_g + P.heur(child)
                    self.came_from[child] = parent
                    if not c_in_a:
                        self.add((f, child))

                    #if child in g and new_g < g[child]:
                    #    came_from[child] = parent
                    #    g[child] = new_g
                    #f_child = new_g + heur(child)
                    #continue
        print "Failure"
        return "Failure"

    def reconstruct_path(self, path):
        " Reconstructs path from goal to start node. "
        while path[-1] != self.P.start:
            path.append(came_from[path[-1]])
        return path[::-1]

    def add(self, elem):
        " Adds element to the agenda (based on search-mode). "
        if self.P.i == 0:
            hq.heappush(self.agenda, elem)
        elif self.P.i == 1:
            self.agenda.insert(0, elem)
        else:
            self.agenda.append(elem)

    def pop(self):
        " Pops from agenda (based on search-mode). "
        if self.P.i == 0:
            return hq.heappop(self.agenda)[1], self.agenda
        else:
            return self.agenda[0][1], self.agenda[1:]

    def contains(self, elem):
        " Does the agenda contain element? "
        return any(map(lambda e:elem == e[1], self.agenda))
