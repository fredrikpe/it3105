#include <iostream>
#include <cstdlib>
#include <time.h>
#include "DynamicMemory.h"
#include "Matrix.h"

#define L 500  

using namespace std;

void multiplyAndAdd(double *data, int r1, int r2, int scalar)
{
    for (int i=0; i<L; i++) {
        data[r1*L + i] *= scalar;
        data[r2*L + i] += data[r1*L + i];
    }
}

void randomize(Matrix &M)
{
    int sign, scalar, r1, r2;
        for (int i=0; i<L*20; i++) {
        sign = (rand() % 2) == 0 ? 1 : -1;
        //scalar = sign * (rand() % 3 + 1);
        r1 = rand() % L;
        r2 = rand() % L;
        multiplyAndAdd(M.data, r1, r2, sign);
    }
}

int main()
{
    srand(time(NULL));
	// Oppgave 2
	{
		Matrix W(L);

        randomize(W);

        Matrix BW(L, 1);
        for (int i=0;i<L;i++)
            BW.set(i, 0, 1);

		cout << L << endl << W << BW;
    }

    /*
    int N;
    cin >> N;
    Matrix A(N);
    cin >> A;
    
    Matrix B(N, 1);
    cin >> B;

    //cout << B;

    Matrix InverseA(N);

    if ( A.Inverse(InverseA) )
    {
        cout << "A:\n" << A
             << "A^-1:\n" << InverseA 
             << "det(A): " << Determinant(A.data, L);
             //<< "\nB:\n" << B
             //<< "I\n" << A*InverseA;

        cout << "The solution is:" << endl << InverseA*B << endl;
    } else {
        cout << "The system has no solution." << endl;
    }
    */

	return 0;
}
