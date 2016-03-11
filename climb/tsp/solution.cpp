
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

using namespace std;

typedef unordered_map<int, int> arc_map;
typedef unordered_map<int, arc_map> graph;
//typedef uint64_t bitset64;

// First permutation with n bits set to 1. 
// n = 3 gives ...000111.
uint64_t firstPermutation(int n)
{
    uint64_t fp = 0UL;
    for (int i=0; i<n; i++) {
        fp |= (1UL << i);
    }
    return fp;
}

// next permutation with same num of 1's.
// 011 -> 101 -> 110
uint64_t nextPermutation(uint64_t prev)
{
    uint64_t t = prev | (prev - 1); // t gets v's least significant 0 bits set to 1
    // Next set to 1 the most significant bit to change, 
    // set to 0 the least significant ones, and add the necessary 1 bits.
    return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctzl(prev) + 1));  
}

void heldKarp(distance_matrix, n)
{
    map<set<int>, int> s
    map<uint64_t, int> first_level;

    for (int k = 0; k < n; k++) {// to n do
        if (k != root) {
            uint64_t set_name = 1 << k;
            first_level[set_name] = distance_matrix[0][k];  //  d1,k
        }
    }
    for (int s = 3; s < n; s++) { 
        map<uint64_t, int> current_level;
        uint64_t current_set = firstPermutation(s);
        uint64_t end = firstPermutation(s-1);
        while (current_set != end) { // for all S ⊆ {1, 2, . . . , n}, |S| = s {
            for (int k=0; k<n; k++) {
                if (current_set & (1UL << k)) { // for all k ∈ S {
                    bitset<64> tmp_set(current_set);
                    cout << "Found node " << k << " in set " << tmp_set << endl;
                   // C(S, k) = minm≠1,m≠k,m∈S [C(S − {k}, m) + dm,k ];
                }
            }

        }
    }

    opt = mink≠1 [C({1, 2, 3, . . . , n}, k) + dk,1]
        return (opt)
}
*/
int main()
{
    
    //set<int> S;
    graph G;

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
                G[p].insert(make_pair(q, dist));
                G[q].insert(make_pair(p, dist));
                break;
            case 'q':
                ss >> root; 
                break;
            default:
                break;
        }
    }

    //for (int n=0; i<N; i++) {
     //   S.insert(n);
    //}
    //S.erase(root);

    //cout << firstPermutation(3) << endl;
    uint64_t a = 1UL << 61 | 1UL << 63 | 1UL << 62; //firstPermutation(34); //1UL << 63;
    bitset<64> x(a);
    cout << x << endl;
    bitset<64> c(nextPermutation(a));
    cout << c << endl;


    //for (auto a:C) {
     //   cout << a.second << endl;
    //}
}
