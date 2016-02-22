

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> wordsTosearch;
int *numOfInstances;

void checkWordList(const string &word) {
    for (int i=0; i<wordsTosearch.size(); i++) {
        if (wordsTosearch[i] == word) {
            numOfInstances[i]++;
            break;
        }
    }
}

void fixString(string &word) {
    for (int i=0; i<word.size(); i++) {
        if ((int) word[i] < 97 || word[i] > 122) {
            word.erase(i, 1);
            i--;
        }
    }
}

int main () {


    string line;
    getline(cin, line);
    stringstream ss(line);
    string word;
    int c = 0;
    while (getline(ss, word, ' ')) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        wordsTosearch.push_back(word);    
        c++;
    }
    numOfInstances = (int*)calloc(c, sizeof(int));

    while (getline(cin, line)) {
        stringstream ss(line);
        string word;
        while (getline(ss, word, ' ')) {
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            fixString(word);
            checkWordList(word);
        }
    }


    for (int i=0; i<c; i++) {
        if (i)
            cout << " ";
        cout << numOfInstances[i];
    }
    cout << endl;
}


