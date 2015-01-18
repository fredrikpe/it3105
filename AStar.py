
import heapq as hq

def a_star(succ, heur, arc_cost, start, is_goal):

    agendaHQ = []
    hq.heappush(agendaHQ, (heur(start), start))
    agenda = [start]
    closed = []
    g = {start:0}
    came_from = {}
    paths = []

    while agenda:
        parent = hq.heappop(agendaHQ)[1]
        agenda.remove(parent)
        closed.append(parent)

        paths.append(reconstruct_path(came_from, parent, start))
        #print agendaHQ
        if is_goal(parent):
            return paths

        for child in succ(parent):
            new_g = g[parent] + arc_cost(parent, child)
            if child in closed:
                continue
            if child not in agenda or (child in g and new_g < g[child]):
                came_from[child] = parent
                g[child] = new_g
                f = new_g + heur(child)
                print f, heur(child)
                if child not in agenda:
                    hq.heappush(agendaHQ, (f, child))
                    agenda.append(child)

                #if child in g and new_g < g[child]:
                #    came_from[child] = parent
                #    g[child] = new_g
                    #f_child = new_g + heur(child)
                #continue
    return "Failure"

def reconstruct_path(came_from, cur, start):
    total_path = [cur]
    while cur != start:
        cur = came_from[cur]
        total_path.append(cur)
    return total_path[::-1]
