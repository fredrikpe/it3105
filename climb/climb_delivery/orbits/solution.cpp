
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

#define EORBIT 365
#define MORBIT 867
#define VORBIT 223

void solve(int e, int m, int v)
{
    int days = 0;
    while (e != 0 || m != 0 || v != 0) {
        int de = EORBIT - e;
        int dm = MORBIT - m;
        int dv = VORBIT - v;
        if (de < dv) {
            if (de < dm) {
                e = 0;
                m += de;
                v += de;
                days += de;
            } else {
                e = (e+dm) % EORBIT;
                m = 0;
                v += dm;
                days += dm;
            }
        } else {
            if (dv < dm) {
                e = (e+dv) % EORBIT;
                m += dv;
                v = 0;
                days += dv;
            } else {
                e = (e+dm) % EORBIT;
                m = 0;
                v = (v+dm) % VORBIT;
                days += dm;
            }
        }
    }
    cout << days << endl;
}

void badsolve(int e, int m, int v) // Takes way too long
{
    int days = 0;
    while (e != 0 || m != 0 || v != 0) {
        e = (e+1) % EORBIT;
        m = (m+1) % MORBIT;
        v = (v+1) % VORBIT;
        days++;
    }
    cout << days << endl;
}

int main() 
{
    int e, m, v;
    cin >> e >> m >> v;

    solve(e, m, v);

    while (cin >> e >> m >> v) {
        solve(e, m, v);
    }
}

