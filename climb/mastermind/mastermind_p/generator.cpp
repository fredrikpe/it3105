

#include <iostream>
#include <time.h>

#define SIZE 20000000 // Approx 20 MB

using namespace std;


int main() {

    srand(time(NULL));

    int c = 1;
    while (c < SIZE) {
       cout << (char) (rand() % 26 + 65);
       c++;
    }
    cout << endl;
    c = 1;
    while (c < SIZE) {
       cout << (char) (rand() % 26 + 65);
       c++;
    }
    cout << endl;
}
