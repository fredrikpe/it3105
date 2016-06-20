
#include <iostream>
#include <math.h>
#include <string>

using namespace std;

uint64_t catalanN(int n) {
    uint64_t num = 1;
    int *factors = (int*)calloc(n+1, sizeof(int));
    for (int k=2; k<=n; k++) {
        if ((n+k) % k == 0) {
            num *= (n+k) / k;
        } else {
            num *= (n + k);
            factors[k] = 1;
        }   
        for (int l=2; l<=k; l++) {
            if (factors[l]) {
                if (num % l == 0) {
                    num /= l;
                    factors[l] = 0;
                }
            }
        }
    }
    free(factors);
    return num;
}


int main() {

    int P;

    string line;

    while (cin >> line) {

        P = stoi(line);
        uint64_t ans = catalanN(P);

        cout << ans << endl;
    }
}

