
#include <sstream>
#include <utility>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

typedef unordered_map<int, int> arc_map;
typedef unordered_map<int, arc_map> graph;

void heldKarp(G, n)
{
  for k := 2 to n do
    C({1, k}, k) := d1,k
  end for

  for s := 3 to n do
    for all S ⊆ {1, 2, . . . , n}, |S| = s do
      for all k ∈ S do
        {C(S, k) = minm≠1,m≠k,m∈S [C(S − {k}, m) + dm,k ]}
      end for
    end for
  end for

  opt := mink≠1 [C({1, 2, 3, . . . , n}, k) + dk,1]
  return (opt)
end

int main()
{
    graph g;

    string line;
    char opt;
    int N, root, p, q, dist, read;

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
                ss >> p >> q >> dist;
                //if (g.find(p) == g.end()) {
                    //g[p] = arc_map();
                //}
                g[p].insert(make_pair(q, dist));
                g[q].insert(make_pair(p, dist));
                break;
            case 'q':
                ss >> root; 
                break;
            default:
                break;
        }
    }

    for (auto a:g) {
        cout << a.first << endl;
    }
}
