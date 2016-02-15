

#include <iostream>
#include <string>
#include <stdio.h>


using namespace std;

int charDistance(char a, char b) {
    int forward = (a < b) ? b - a : a - b;
    return (forward <= 13) ? forward : 26 - forward; 
}

int editDistance(string s, string t) {
    int dist = 0;
    for (int i=0; i<s.size(); i++) {
        dist += charDistance(s[i], t[i]);
    }
    return dist;
}

int main() {

    ios_base::sync_with_stdio(false);

    string solution;
    string attempt;
    cin >> solution;
    cin >> attempt;

    cout << editDistance(solution, attempt) << endl;


}
