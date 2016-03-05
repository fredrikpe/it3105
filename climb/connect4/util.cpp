
#include "util.h"

void Node::print_board()
{
    for (int i=0; i<H; i++) {
        for (int j=0; j<W; j++) {
            std::cout << board[i*W + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/*
std::istream& operator>>( std::istream& is, Disc& i )
{
    int tmp ;
    if ( is >> tmp )
        i = static_cast<Disc>( tmp ) ;
    return is ;
}
*/
std::istream& operator>>(std::istream &in, Node &n)
{
    for (int i=0; i<H; i++) {
        for (int j=0; j<W; j++) {
            int tmp;
            in >> tmp;
            n.board[i*W + j] = tmp;
        }
    }
}

Node::Node(Disc c) : color(c) {}

Node::Node(Disc *b, Disc c)
{
    color = c;
    for (int i=0; i<H; i++) {
        for (int j=0; j<W; j++) {
            board[i*W + j] = b[i*W + j];
        }
    }
}

int Node::insert(int j)
{
    int pos = -1;
    for (int i=0; i<H; i++) {
        if (board[i*W + j] != 0) {
            return pos;
        }
        pos++;
    }
}

void Node::generate_children()
{
    int child_color = color == 1 ? 2 : 1;
    for (int j=0; j<W; j++) {
        int tmp_ins = insert(j);
        if (tmp_ins != -1) {
            Node child(board, child_color); 
            child.board[tmp_ins*W + j] = child_color;
            children.push_back(child);
        }
    }
}



static int find_diagonal(Disc *board, int size, Disc color)
{
    int total = 0;
    for (int i=0; i<W+H; i++) {
        int s1 = 0;
        int s2 = 0;
        for (int j=0; j<H; j++) {
            int imin = i-j;
            int iplus = i-(H-1-j);
            if (imin < W && imin >= 0) {
                s1 = (board[imin*W + j] == color) ? s1 + 1 : 0;
                if (s1 == size) {
                    total += 1;
                    s1 = 0;
                }
            }
            if (iplus < W && iplus >= 0) {
                s2 = (board[iplus*W + j] == color) ? s2 + 1 : 0;
                if (s2 == size) {
                    total += 1;
                    s2 = 0;
                }
            }
        }
    }
    return total;
}


static int find_straight(Disc *board, int size, Disc t, bool vertical)
{
    int A = vertical ? H : W;
    int B = vertical ? W : H;
        
    int total = 0;
    for (int i=0; i<B; i++) {
        int s = 0;
        for (int j=0; j<A; j++) {
            s = (board[i*W + j] == t) ? s + 1 : 0;
            if (s == size) {
                total += 1;
                s = 0;
            }
        }
    }
    return total;
}

int find(Disc *board, int size, Disc t)
{
    return find_straight(board, size, t, true) 
         + find_straight(board, size, t, false) 
         + find_diagonal(board, size, t);
}
