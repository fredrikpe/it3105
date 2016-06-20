
#include <iostream>
#include <math.h>
#include <string>
#include <map>

using namespace std;

int M;
int N;

map<int, int> graph;

void solve()
{
    
}

int main() 
{
    cin >> M >> N;
    string line;

    int a, b;
    for (int i=0; i<M; i++) {
        cin >> a >> b;
        graph[a].insert(b);
    }
    solve();

    while (cin >> M >> N) {
        graph.clear();
        for (int i=0; i<M; i++) {
            cin >> a >> b;
            graph[a].insert(b);
        }
        solve();
    }
}

