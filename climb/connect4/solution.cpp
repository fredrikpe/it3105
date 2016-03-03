

#include <iostream>
#include <algorithm>

enum Color {
    RED;
    BLUE;
};

class Connect4 {
    int board[7*6];
public:
    Connect4();

    void placeDisc(int pos, Color c);
};

int main() {

    Connect4 c4;

    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        string word;
        while (getline(ss, word, ' ')) {
            n = stol(word);
        }
    }



    return 0;
}
