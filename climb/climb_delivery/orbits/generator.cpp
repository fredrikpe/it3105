

#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

int M = 200;

int main()
{
    srand(time(NULL));

    int i;
    for (i=0; i<M; i++) {
        cout << rand() % 365 << " " << rand() % 685 << " " << rand() % 545 << endl;
    }
}
