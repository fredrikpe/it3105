
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>

using namespace std;

vector<string> wordsTosearch;
int *numOfInstances;

void checkWordList(string word) {
    for (int i=0; i<wordsTosearch.size(); i++) {
        if (wordsTosearch[i] == word) {
            numOfInstances[i]++;
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
        wordsTosearch.push_back(word);    
        c++;
    }
    numOfInstances = (int*)calloc(c, sizeof(int));

    while (getline(cin, line)) {
        stringstream ss(line);
        string word;
        while (getline(ss, word, ' ')) {
            switch word {
case :w

        }
    }


    for (int i=0; i<c; i++) {
        if (i)
            cout << " ";
        cout << numOfInstances[i];
    }
    cout << endl;
}

