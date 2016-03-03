
#include <iostream>

using namespace std;

#define _N 16
#define _M 4


class Sudoku {
   int cells[_N*_N];

public:
   Sudoku();

   void     insert_line(string line, int row_nr);
   bool     insert(int val, int pos);
   bool     legal_move(int val, int pos);
   bool     legal_board();
   void     write_mr(ostream& o) const;
   void     write_hr(ostream& o) const;

   void     solve_insert(int val, int pos);
   bool     brute_solve();
};


// Human readable output
void Sudoku::write_hr(ostream& o) const {
    o << "--------------------------------\n";
    for (int i=0; i< _N*_N; i++) {
        if (cells[i] > 9) o << (char) (cells[i] + 55);
        else if (cells[i] == 0) o << " ";
        else o << cells[i];

        if (i % _N == _N - 1) {
            o << endl;
            if ((i/_N) % _M == _M - 1) 
                o << "--------------------------------\n";
        } else if (i % _M == _M - 1) o << "|";
        else o << " ";

    }
}

void Sudoku::write_mr(ostream& o) const {
    for (int i=0; i< _N*_N; i++) {
        if (cells[i] > 9) o << (char) (cells[i] + 55);
        else o << cells[i];

        if (i % _N == _N - 1) o << endl;
        else o << " ";
    }
}

bool Sudoku::legal_move(int val, int pos) {
    int row_nr = pos / _N;
    int col_nr = pos % _N;

    int box_r = row_nr / _M; 
    int box_c = col_nr / _M; 
    int box_pos;

    for (int i=0; i<_N; i++) {
        // check row
        if (cells[row_nr*_N + i] == val && (row_nr*_N + i) != pos) {
            return false;
        }
        // column
        if (cells[col_nr + i*_N] == val && (col_nr + i*_N) != pos) {
            return false;
        }    
        // box  
        box_pos = (box_r*_M)*_N + box_c*_M + (i/_M)*_N + i%_M; 
        if (cells[box_pos] == val && box_pos != pos) {
            return false;
        }
    }
    return true;
}

bool Sudoku::insert(int val, int pos) {
    if (legal_move(val, pos)) {
        cells[pos] = val;
        return true;
    }
    return false;
}

Sudoku::Sudoku() {
    for (int i=0; i<_N*_N; i++)
        cells[i] = 0;
}


void Sudoku::insert_line(string s, int row_nr) {
   int c = 0;
   for (int i = 0; i < s.size(); i++) {
      if (s[i] >= '1' && s[i] <= '9') {
          insert((int)s[i] - 48, c++ + _N*row_nr); 
      } else if (s[i] >= 'A' && s[i] <= 'G') {
          insert((int)s[i] - 55, c++ + _N*row_nr);
      } else if (s[i] == '0') c++;
   }
}


bool Sudoku::brute_solve() {
    for (int i=0; i<_N*_N; i++) {
        if (this->cells[i] == 0) {
            for (int val=1; val<=_N; val++) {
                if (legal_move(val, i)) {
                    cells[i] = val;
                    if (!brute_solve()) 
                        cells[i] = 0;
                    else 
                        return true;
                }
            }
            return false;
        }
    }
    return true;
}

bool Sudoku::legal_board() {
   for (int i=0; i<_N*_N; i++) {
       if (!legal_move(cells[i], i))
           return false;
   }
   return true;
} 

int main() {
   Sudoku sdk;

   string line;
   int row_nr = 0;
   while (getline(cin, line)) {
       sdk.insert_line(line, row_nr++);
   }
   //sdk.write_hr(cout);
   sdk.brute_solve();

   sdk.write_mr(cout);
}
