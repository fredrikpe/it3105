// C++ Greedy TSP solver using nearest neighbour algorithm (exercise-2)

#include <iostream>
#include <fstream>
#include <limits>
#include <assert.h>

#include <algorithm>
#include <math.h>
#include <vector>

using namespace std;


// Rudimentary vertex struct
struct Vertex {
    int id, x, y;
};


// Overloads == operator for std::find
bool operator==(const Vertex& a, const Vertex& b) {
    // Redundant checking for fun
    return (a.id == b.id && a.x == b.x && a.y == b.y);
}


// Checks to see if v resides in vec
bool contains(std::vector<Vertex> vec, Vertex v) {
    return std::find(vec.begin(), vec.end(), v) != vec.end();
}


// Finds the appropriate Vertex struct given an id
Vertex getBasedOnId(std::vector<Vertex> vec, int id) {
    for (unsigned int i = 0; i < vec.size(); i++)
        if (vec[i].id == id)
            return vec[i];
    assert(false);  // We would ideally not like to be here
}


// Calculates the Euclidean distance between v1 and v2
double euclideanDistance(Vertex v1, Vertex v2) {
    double dX = v1.x - v2.x;
    double dY = v1.y - v2.y;
    return sqrt(dX*dX + dY*dY);
}


// Finds the nearest (not yet explored) vertex to v
Vertex getNearestVertex(std::vector<Vertex> vec,
                        std::vector<Vertex> seen,
                        Vertex v) {
    Vertex nearest = {-1, 0, 0};
    double dist = std::numeric_limits<double>::infinity();

    for (unsigned int i = 0; i < vec.size(); i++) {
        if (contains(seen, vec[i])) {
            continue;
        }
        double tmp = euclideanDistance(v, vec[i]);
        if (tmp < dist) {
            dist = tmp;
            nearest = vec[i];
        }
    }
    return nearest;
}


// Rudimentary implementation of a TSP solver using nearest neighbour
std::vector<Vertex> tspNN(std::vector<Vertex> vertices, int source) {
    // Create path vector and add the source vertex
    std::vector<Vertex> path;
    Vertex s = getBasedOnId(vertices, source);
    Vertex current = s;
    path.push_back(current);

    bool isDone = false;
    while (isDone == false) {
        current = getNearestVertex(vertices, path, current);
        if (current.id == -1) {
            path.push_back(s);
            isDone = true;
        } else {
            path.push_back(current);
        }
    }

    return path;
}


// Prints out the results to stdout
void printResults(std::vector<Vertex> path) {
    cout << "s ";
    for (unsigned int i = 0; i < path.size(); i++) {
        cout << path[i].id << " ";
    }
    cout << endl;
}


// Driver program
int main() {
    cin.sync_with_stdio(false);
    string code;
    int source, id, x, y, N;
    std::vector<Vertex> vertices;

    while (cin >> code) {
        if (!code.compare("c")) {  // Skip comments
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        } else if (!code.compare("p")) {  // Read number of vertices into N
            cin >> N;
            continue;
        } else if (!code.compare("v")) {
            cin >> id >> x >> y;
            Vertex vertex = {id, x, y};
            vertices.push_back(vertex);
            continue;
        } else if (!code.compare("q")) {
            cin >> source;
            std::vector<Vertex> path = tspNN(vertices, source);
            printResults(path);
            continue;
        }
        // Should never get here with the correct input
        assert(false);
    }

    return 0;
}
