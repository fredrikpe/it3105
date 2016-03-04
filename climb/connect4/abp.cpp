
#include <iostream>
#include <string>
#include <climits>

#include "util.h"



int heuristic(Node node)
{
    int f = find(node.board, 4, node.color) ;
    //if (f) std::cout << f;
    return W4 * f 
         + W3 * find(node.board, 3, node.color) 
         + W2 * find(node.board, 2, node.color);
}

void cut(Node node, int num)
{
    node.children.erase(node.children.begin() + num + 1, node.children.end());
}

int alphabeta(Node node, int depth, int alpha, int beta) 
{
    node.generate_children();
    //std::cout << node.children.size() << " ";
    if(depth == 0 || node.children.empty()) {
        int value = heuristic(node);
        return node.color == 1 ? value : -value;
    }
    int temp;
    int v;
    if(node.color == 1) {
        v = -INT_MAX; 
        for (int i=0; i<node.children.size(); i++) {
            v = max(v, alphabeta(node.children[i], depth-1, alpha, beta));
            alpha = max(alpha, v);
            if(beta <= alpha) {
                cut(node, i);   //beta cut-off  
            }
        }
        std::cout << v << " ";
        return v;
    } else {
        v = INT_MAX;
        for (int i=0; i<node.children.size(); i++) {
            v = max(v, alphabeta(node.children[i], depth-1, alpha, beta));
            beta = min(beta, v);
            if(beta <= alpha) {
                cut(node, i);   //alpha cut-off         
            }
        }
        return v;
    }
}

int main()
{
    Node root(1);
    std::cin >> root;
    root.print_board();

    std::cout << alphabeta(root, 83, -INT_MAX, INT_MAX) << std::endl;
}




