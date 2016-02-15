

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <stdio.h>

#define NUM_DIGITS 11

using namespace std;

typedef unsigned long int u64;



u64 lastNDigits(u64 num) {
    return num % (u64) pow(10, NUM_DIGITS);
}

int main() {

    string line;
    u64 current_digits = 1; 
    u64 n = 1;
    while (getline(cin, line)) {
        stringstream ss(line);
        string word;
        while (getline(ss, word, ' ')) {
            n = stol(word);
            if (n & 1 && !(n%11)) {
                current_digits *= n;
                current_digits = lastNDigits(current_digits);
            }
        }
    }

    current_digits = lastNDigits(current_digits);

    cout << current_digits << endl;

    return 0;
}




