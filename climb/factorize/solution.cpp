

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <stdio.h>

#define NUM_DIGITS 11

using namespace std;

typedef unsigned long int u64;

const u64 FLAG = (u64)-1 << 56;


u64 lastNDigits(u64 num, int n) {
    return num % (u64) pow(10, n);
}

int main() {

    ios_base::sync_with_stdio(false);

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
                if (current_digits & FLAG) 
                    current_digits = lastNDigits(current_digits, NUM_DIGITS);
            }
        }
    }

    current_digits = lastNDigits(current_digits, NUM_DIGITS);

    cout << current_digits << endl;

    return 0;
}




