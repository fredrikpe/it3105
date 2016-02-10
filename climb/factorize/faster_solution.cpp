

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <stdio.h>

#define NUM_DIGITS 11

using namespace std;

typedef unsigned long int u64;


u64 lastNDigits(u64 number, int n) {
    return number % 100000000000;
}

int main() {

    ios_base::sync_with_stdio(false);

    string line;
    u64 current_digits = 0; 
    int i = 0;
    while (getline(cin, line)) {
        stringstream ss(line);
        string word;
        while (getline(ss, word, ' ')) {
            current_digits += stol(word);
        }
    }

    current_digits = lastNDigits(current_digits, NUM_DIGITS);

    cout << current_digits << endl;

    return 0;
}




