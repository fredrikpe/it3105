
import heapq as hq

def a_star(succ, heur, arc_cost, start, is_goal, mode="astar"):
    """ A general A* algorithm

    arguments:
    the graph, given by its successor function,  node -> [node]
    heuristic function,                          node -> num,
    distance cost funtion,                       node, node -> num,
    starting node,                               node
    is_goal function.                            node -> bool

    """
    if mode == "astar": i = 0
    elif mode == "dfs": i = 1
    elif mode == "bfs": i = 2
    else: print "Error: Unknown mode\n(Modes: astar, dfs, bfs)"; return
    agendaQ = []
    add(agendaQ, (heur(start), start), i)
    agenda = [start]
    closed = []
    g = {start:0}
    came_from = {}
    paths = []

    while agenda:

        parent, agendaQ = pop(agendaQ, i)
        agenda.remove(parent)
        closed.append(parent)
        paths.append(reconstruct_path(came_from, parent, start))

        if is_goal(parent):
            return paths

        for child in succ(parent):

            new_g = g[parent] + arc_cost(parent, child)
            if child in closed:
                continue

            if child not in agenda or (child in g and new_g < g[child]):
                g[child] = new_g
                f = new_g + heur(child)
                came_from[child] = parent

                if child not in agenda:
                    add(agendaQ, (f, child), i)
                    agenda.append(child)

                #if child in g and new_g < g[child]:
                #    came_from[child] = parent
                #    g[child] = new_g
                    #f_child = new_g + heur(child)
                #continue
    print "Failure"
    return "Failure"

def reconstruct_path(came_from, cur, start):
    total_path = [cur]
    while cur != start:
        cur = came_from[cur]
        total_path.append(cur)
    return total_path[::-1]

def add(agenda, n, i):
    if i == 0:
        hq.heappush(agenda, n)
    elif i == 1:
        agenda.insert(0,n)
    else:
        agenda.append(n)

def pop(agenda, i):
    if i == 0:
        return hq.heappop(agenda)[1], agenda
    else:
        return agenda[0][1], agenda[1:]
