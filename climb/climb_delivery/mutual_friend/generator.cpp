

#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

int N = 100;
int M = 40000;

int main()
{
    srand(time(NULL));

    cout << M << " " << N << endl;
    int i, j;
    for (i=0; i<M; i++) {
    for (j=0; j<N; j++) {
        if (rand() % 10 > 1) {cout << '#'; }else {cout << '-';}
    }
    cout << endl;
    }
    cout << M << " " << N << endl;
    for (i=0; i<M; i++) {
    for (j=0; j<N; j++) {
        if (rand() % 10 > 1) {cout << '#'; }else {cout << '-';}
    }
    cout << endl;
    }
    cout << M << " " << N << endl;
    for (i=0; i<M; i++) {
    for (j=0; j<N; j++) {
        if (rand() % 10 > 1) {cout << '#'; }else {cout << '-';}
    }
    cout << endl;
    }
    cout << endl;
}
