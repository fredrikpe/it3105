

#include <iostream>

using namespace std;


const int DIM = 16;

class Sudoku {
   int cells[DIM*DIM];
   // More stuff?

public:
   Sudoku();

   // TODO
   // void     insert(value... position?);
   // bool     legal_move(value...);
   // void     solve();
   // void     insert_line(string line... );
   // More stuff?
   void     write(ostream& o) const;

};

void Sudoku::write(ostream& o) const {
    for (int i=0; i< DIM*DIM; i++) {
        if (cells[i] > 9) o << (char) (cells[i] + 55);
        else o << cells[i];

        if (i % DIM == DIM - 1) o << endl;
        else o << " ";
    }
}

Sudoku::Sudoku() {
    for (int i=0; i<DIM*DIM; i++)
        cells[i] = 0;
}

int main() {
   Sudoku sdk;

   string line;
   while (cin >> line)) {
       // TODO:
   }
   // sdk.solve();

   // sdk.write(cout);
}
