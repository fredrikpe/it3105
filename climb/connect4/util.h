
#include <iostream>
#include <vector>

#define W4 1000
#define W3 10
#define W2 1

#define H 6
#define W 7



typedef int Disc;
/*enum Disc {
    empty,
    red,
    blue,
};*/

class Node {
public:
    Disc color;
    Disc board[H*W];
    std::vector<Node> children;
    Node(Disc *b, Disc c);
    Node(Disc c);
    int insert(int j);
    void generate_children();
    void print_board();
    friend std::istream& operator>>(std::istream &in, Node &n);
};

inline int max(int a, int b)
{
    return (a > b) ? a : b;
}

inline int min(int a, int b)
{
    return (a < b) ? a : b;
}

static int find_diagonal(Disc *board, int size, Disc color);

static int find_straight(Disc *board, int size, Disc t, bool vertical);

int find(Disc *board, int size, Disc color);

