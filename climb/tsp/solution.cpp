
#include <bitset>
#include <sstream>
#include <utility>
#include <unordered_map>
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

struct Edge {
    int start;
    int end;
    double dist;
};



typedef unordered_map<int, double> arc_map;

typedef vector<Vertex> VertexGraph;
typedef unordered_map<int, arc_map> EdgeGraph;



int N;

double distance(const Vertex v1, const Vertex v2) // test for inline
{
    double dx = v1.x - v2.x;
    double dy = v1.y - v2.y;
    return sqrt(dx*dx + dy*dy);
}

void insertSmallestEdge(const VertexGraph &graph, EdgeGraph &edge_graph, vector<int> &remaining, bool from_edge_graph)
{
    int end_node, start_node = -1;
    int ei, si;
    double cur_distance = 2000000; // fix
    for (int i=0; i<remaining.size(); i++) { //auto new_node : remaining) {
        if (from_edge_graph) {
            for (auto node : edge_graph) {
                double new_distance = distance(graph[node.first], graph[remaining[i]]); 
                if (new_distance < cur_distance) {
                    cur_distance = new_distance;
                    start_node = node.first;
                    end_node = remaining[i];
                    ei = i;
                }
            }
        } else {
            for (int j=i+1; j<remaining.size(); j++) {
                double new_distance = distance(graph[remaining[j]], graph[remaining[i]]); 
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

    edge_graph[start_node].insert(make_pair(end_node, cur_distance));
    edge_graph[end_node].insert(make_pair(start_node, cur_distance));

    remaining.erase(remaining.begin() + ei);
    if (!from_edge_graph) remaining.erase(remaining.begin() + si - 1);//, remaining.end(), end_node), remaining.end());
}
/*
    edge_tree[start_node].insert(make_pair(remaining[end_node], cur_distance));
    edge_tree[remaining[end_node]] = arc_map();
    edge_tree[remaining[end_node]].insert(make_pair(start_node, cur_distance));

    remaining.erase(remaining.begin() + end_node);//, remaining.end(), end_node), remaining.end());
}*/

void minimalSpanningTree(const VertexGraph &graph, EdgeGraph &edge_tree)
{
    edge_tree[0] = arc_map();
    vector<int> remaining;
    for (int i=1; i<N; i++) {
        remaining.push_back(i);
    }
    while (edge_tree.size() < N) {
        insertSmallestEdge(graph, edge_tree, remaining, true);
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

void perfectMatching(EdgeGraph &match_graph, const VertexGraph &graph, const vector<int> &odd_degree)
{
    vector<int> remaining;
    for (auto n : odd_degree) {
        remaining.push_back(n);
    }
    while (match_graph.size() < odd_degree.size()) {
        insertSmallestEdge(graph, match_graph, remaining, false);
    }
}

void combinePM_MST(EdgeGraph &com, const EdgeGraph &pm, const EdgeGraph &mst)
{
    for (auto node : mst) {
        com[node.first] = node.second;
    }
    for (auto node : pm) {
        for (auto arc : node.second) {
            com[node.first].insert(arc);
        }
    }
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
                root--; // 0 indexed
                break;
            default:
                break;
        }
    }

    EdgeGraph mst;
    minimalSpanningTree(G, mst);

    vector<int> od;
    oddDegree(mst, od);

    EdgeGraph pm;
    perfectMatching(pm, G, od);

    EdgeGraph com;
    combinePM_MST(com, pm, mst);

    cout << "Minimal spanning tree (size = " << mst.size() << ")\n";
    for (auto a:mst) {
        cout << a.first << " degree: " << a.second.size() << endl;
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
        if (node.second.size() % 2 == 1) 
            cout << "node: " << node.first << "degree: " << node.second.size() << "\n";
    }
}
