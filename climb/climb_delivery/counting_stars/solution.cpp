
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

int M;
int N;

vector<string> bitmap;

void changeSurrounding(int x, int y) {
    bitmap[x][y] = '#';
    if (x > 0 && bitmap[x-1][y] == '-') {
        changeSurrounding(x-1, y);
    }
    if (y > 0 && bitmap[x][y-1] == '-') {
        changeSurrounding(x, y-1);
    }
    if (x < M-1 && bitmap[x+1][y] == '-') {
        changeSurrounding(x+1, y);
    }
    if (y < N-1 && bitmap[x][y+1] == '-') {
        changeSurrounding(x, y+1);
    }
}
    
void findStars()
{
    int stars = 0;
    for (int i=0; i<M; i++) {
        for (int j=0; j<N; j++) {
            if (bitmap[i][j] == '-') {
                stars++;
                changeSurrounding(i, j);
            }
        }
    }
    cout << stars << endl;
}


int main() 
{
    cin >> M >> N;
    string line;

    for (int i=0; i<M; i++) {
        cin >> line;
        bitmap.push_back(line);
    }
    findStars();

    while (cin >> M >> N) {
        bitmap.clear();
        for (int i=0; i<M; i++) {
            cin >> line;
            bitmap.push_back(line);
        }
        findStars();
    }
}

