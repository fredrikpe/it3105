
#include <bitset>
#include <sstream>
#include <utility>
#include <unordered_map>
#include <queue>
#include <map>
#include <algorithm>
#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>


using namespace std;

struct Vertex {
    int x;
    int y;
};

class CompareDist
{
public:
    bool operator()(pair<pair<int,int>, float> n1, pair<pair<int,int>, float> n2) {
        return n1.second>n2.second;
    }
};

typedef vector<pair<int, float>> arc_map;
typedef vector<Vertex> VertexGraph;
typedef unordered_map<int, arc_map> EdgeGraph;
typedef vector<vector<float>> DistanceMatrix;
typedef priority_queue<pair<pair<int, int>, float>, vector<pair<int, int>, float>, CompareDist> DistanceQueue;

int N;

float distance(const Vertex v1, const Vertex v2) // test for inline
{
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    return sqrt(dx*dx + dy*dy);
}

void computeDistanceQueue(DistanceQueue &dq, VertexGraph &graph)
{
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            dq.push(make_pair(make_pair(i, j), distance(graph[i], graph[j])));
        }
    }
}

void computeDistanceMatrix(DistanceMatrix &dm, VertexGraph &graph)
{
    for (int i=0; i<N; i++) {
        vector<float> a;
        dm.push_back(a);
        for (int j=0; j<N; j++) {
            if (j != i) {
                dm[i].push_back(distance(graph[i], graph[j]));
            } else {
                dm[i].push_back(0);
            }
        }
    }
}

void insertSmallestEdge(const DistanceMatrix &dist_mat, EdgeGraph &edge_graph, vector<int> &remaining, bool from_edge_graph)
{
    int end_node, start_node = -1;
    int ei, si;
    float cur_distance = 2000000; // fix
    for (int i=0; i<remaining.size(); i++) { //auto new_node : remaining) {
        if (from_edge_graph) {
            for (auto node : edge_graph) {
                float new_distance = dist_mat[node.first][remaining[i]]; 
                if (new_distance < cur_distance) {
                    cur_distance = new_distance;
                    start_node = node.first;
                    end_node = remaining[i];
                    ei = i;
                }
            }
        } else {
            for (int j=i+1; j<remaining.size(); j++) {
                float new_distance = dist_mat[remaining[j]][remaining[i]]; 
                if (new_distance < cur_distance) {
                    cur_distance = new_distance;
                    start_node = remaining[j];
                    end_node = remaining[i];
                    ei = i; si = j;
                }
            }
        }
    }
    if (start_node == -1) cout << "ERROR: No edge found!\n";

    if (!from_edge_graph) edge_graph[start_node] = arc_map();
    edge_graph[end_node] = arc_map();

    edge_graph[start_node].push_back(make_pair(end_node, cur_distance));
    edge_graph[end_node].push_back(make_pair(start_node, cur_distance));

    remaining.erase(remaining.begin() + ei);
    if (!from_edge_graph) remaining.erase(remaining.begin() + si - 1);//, remaining.end(), end_node), remaining.end());
}

void minimalSpanningTree(const DistanceMatrix &dist_mat, EdgeGraph &edge_tree)
{
    edge_tree[0] = arc_map();
    vector<int> remaining;
    for (int i=1; i<N; i++) {
        remaining.push_back(i);
    }
    while (edge_tree.size() < N) {
        insertSmallestEdge(dist_mat, edge_tree, remaining, true);
    }
}

void oddDegree(const EdgeGraph &edge_tree, vector<int> &odd_degree) 
{
    for (auto node : edge_tree) {
        if (node.second.size() % 2 == 1) { // Odd
            odd_degree.push_back(node.first);
        }
    }
}

void perfectMatching(EdgeGraph &match_graph, const DistanceMatrix &dist_mat, const vector<int> &odd_degree)
{
    vector<int> remaining;
    for (auto n : odd_degree) {
        remaining.push_back(n);
    }
    while (match_graph.size() < odd_degree.size()) {
        insertSmallestEdge(dist_mat, match_graph, remaining, false);
    }
}

void combinePM_MST(EdgeGraph &com, const EdgeGraph &pm, const EdgeGraph &mst)
{
    for (auto node : mst) {
        com[node.first] = node.second;
    }
    for (auto node : pm) {
        for (auto arc : node.second) {
            com[node.first].push_back(arc);
        }
    }
}

void removeEdge(EdgeGraph &g, int start, int end)
{
    for (int i=0; i<g[start].size(); i++) {
        if (g[start][i].first == end) {
            g[start].erase(g[start].begin() + i);
            return;
        }
    }
}

void addExtraPath(vector<int> &euler_path, const int root, EdgeGraph &com)
{
    int new_node, cur_node = root;
    do {
        new_node = com[cur_node].back().first;
        // Removing edges
        com[cur_node].pop_back();
        removeEdge(com, new_node, cur_node);
        // Add to path
        euler_path.push_back(new_node);
        cur_node = new_node;
    } while (cur_node != root);
}

void createEulerPath(vector<int> &euler_path, const int root, EdgeGraph &com)
{
    euler_path = {root};
    vector<int> remaining;
    for (int i=0; i<N; i++) {
        if (i != root)
            remaining.push_back(i);
    }
    int new_node, cur_node = root;
    //cout << "Root degree: " << com[root].size() << endl;
    while (remaining.size() > 0) {
        if (com[cur_node].size() == 0) {
            for (auto node : com) {
                if (node.second.size() > 0) {
                   addExtraPath(euler_path, node.first, com);
                }
            }
            return;
        }
        //if (cur_node == root) cout << "c: " << c << " at root\n";
        new_node = com[cur_node].back().first;
        // Removing edges
        com[cur_node].pop_back();
        removeEdge(com, new_node, cur_node);
        // Removing visited
        remaining.erase(remove(remaining.begin(), remaining.end(), new_node), remaining.end());
        // Add to path
        euler_path.push_back(new_node);
        cur_node = new_node;
    }
}       

void shortcut(vector<int> &path, int node)
{
    for (int i=0; i<path.size(); i++) {
        if (path[i] == node) {
            path.erase(path.begin() + i);
            return;
        }
    }
}

void createTSP(vector<int> &tsp, vector<int> euler_path, const int root) 
{
    bool visited[N];
    for (int i=0; i<N; i++) {
        visited[i] = false;
    }
    for (int i=0; i<euler_path.size(); i++) { //auto node : euler_path) {
        if (visited[euler_path[i]]) {
            shortcut(euler_path, euler_path[i--]);
        } else {
            visited[euler_path[i]] = true;
        }
    }   
    auto it = find(euler_path.begin(), euler_path.end(), root);
    while (tsp.size() < N) {
        tsp.push_back(*it);
        it++;
        if (it == euler_path.end()) {
            it = euler_path.begin();
        }
    }
    tsp.push_back(root);
}

void printTSP(const vector<int> &tsp)
{
    cout << "s ";
    for (auto node : tsp) {
        cout << node + 1 << " ";
    }
    cout << endl;
}

int main()
{
    VertexGraph G;

    string line;
    char opt;
    int root, id, x, y, read;

    while (getline(cin, line)) {
        stringstream ss(line);
        ss >> opt;
        switch (opt) {
            case 'c':
                break;
            case 'p':
                ss >> N;
                break;
            case 'v':
                ss >> id >> x >> y;
                G.push_back(Vertex{x, y});
                break;
            case 'q':
                ss >> root; 
                root--;
                break;
            default:
                break;
        }
    }

    cout << "graph length: " << G.size() << endl;
    DistanceMatrix dm;
    computeDistanceMatrix(dm, G);

    EdgeGraph mst;
    minimalSpanningTree(dm, mst);

    vector<int> od;
    oddDegree(mst, od);

    EdgeGraph pm;
    perfectMatching(pm, dm, od);

    EdgeGraph com;
    combinePM_MST(com, pm, mst);

    vector<int> ep;
    createEulerPath(ep, root, com);

    vector<int> tsp;
    createTSP(tsp, ep, root);

    cout << "TSP size: " << tsp.size() << endl;
    printTSP(tsp);

    // Testing
    /*
    cout << "Minimal spanning tree (size = " << mst.size() << ")\n";
    for (auto a:mst) {
        cout << a.first << " degree: " << a.second.size() << endl;
        for (auto arc:a.second) {
            cout << arc.first << " ";
        }
        cout << endl;
    }
    cout << "\n\nOdd degree graph (size = " << od.size() << ")\n";
    for (auto a:od) {
        cout << a << " ";
    }
    cout << "\n\nPerfect matching:\n";
    for (auto a:pm) {
        for (auto arc : a.second) {
            cout << a.first << "  to  " << arc.first << "\tDist: " << arc.second;
        }
        cout << endl;
    }
    cout << "\nMultigraph H (size = " << com.size() << ")\n";
    for (auto node : com) {
        cout << "node: " << node.first << "degree: " << node.second.size() << "\n";
    }
    cout << "\nEuler path (size = " << ep.size() << ")\n";
    for (auto node : ep) {
        cout << node << " ";
    }
    cout << "\nTSP path (size = " << tsp.size() << ")\n";
    for (auto node : tsp) {
        cout << node << " ";
    }
    cout << endl;
    */ // End testing.
}
