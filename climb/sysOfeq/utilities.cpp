#include <iostream>
#include "utilities.h"

using namespace std;

void printArray(int array[], int length) {
    for (int i = 0; i < length; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}
