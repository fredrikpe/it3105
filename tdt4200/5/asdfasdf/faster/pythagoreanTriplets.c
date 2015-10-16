#include <stdio.h> // for stdin
#include <stdlib.h>
#include <unistd.h> // for ssize_t
#include <math.h>

#ifdef HAVE_MPI
#include <mpi.h>
#endif

#ifdef HAVE_OPENMP
#include <omp.h>
#endif


/*int coPrime1(int x, int y) {
	while (x != 0 && y != 0) {
		if (x >= y)
			x = x % y;
		else
			y = y % x;
	}
	return (x + y) == 1;
}*/
int gcd(int a, int b)
{
	while (1) {
		a %= b;
		if (a == 0) return b;
		b %= a;
		if (b == 0) return a;
	}
}

//int primes[5] = {3,5,7,11,13};


int * factors(int n) {
	int *f = (int*)calloc(5, sizeof(int));
	int c = 0;
	int tmp = n;
	for (int i=3;i<10000;i++) {
		if (i <= tmp) {
			if (tmp % i == 0) {
				f[c++] = i;
				tmp = tmp / i;
			}
		} else {
			break;
		}
	}
	int *d = (int*)calloc(c+1, sizeof(int));
	int b= 1;
	d[0] = c;
	for (int i=0;i<5;i++) {
		if (f[i] != 0) {
			d[b++] = f[i];
		}
	}
	return d;
}



int run(int start, int stop, int numThreads, int mystart, int myend) {
	if (numThreads <= 0 || numThreads > 10) // First nt_value is sometimes way off idk why.
		numThreads = 1;
	int sum = 0;

	if (mystart == 0) mystart++;

	#pragma omp parallel for num_threads(numThreads) reduction(+:sum)
	for (int n=mystart; n<myend; n++) {
		int *d = factors(n);
		int *d_c = factors(n);
		for (int i=1;i<=d[0];i++) {
			d[i] += n;
		}
		for (int m=n+1; m<myend; m+=2) {
			int b = 1;
			for (int i=1;i<=d[0];i++) {
				if (m == d[i]) {
					d[i] += d_c[i]*2;
					b = 0;
				}
			}
			if (b) {
				if (m*m + n*n >= start && m*m + n*n < stop) {
					sum++;
				}
			}
		}
	}


	/*
	#pragma omp parallel for num_threads(numThreads) reduction(+:sum)
	for (int a=3; a<stop-1; a++) {
		for (int b=a+1; b<stop; b+=2) {
			if (coPrime(a, b)) {
				double z = (double) (a*a + b*b);
				double c = sqrt(z);
				int root_z = (int)c;
				if (root_z * root_z == z &&  c >= start && c < stop) {
					sum++;
				}
			}
		}
	}*/
	return sum;
}



int main(int argc, char **argv) {


	int rank, size;
	int *start, *stop, *numThreads;
	int amountOfRuns = 0;
	int aquired;

	int *sum;
	int *total_sum;

	#ifdef HAVE_OPENMP
	omp_set_dynamic(0); // Is this needed?
	#endif

	#ifdef HAVE_MPI
	#ifdef HAVE_OPENMP
	// If both MPI and OpenMP is available / present
	MPI_Init_thread(&argc, &argv , MPI_THREAD_MULTIPLE , &aquired );
	MPI_Comm_rank ( MPI_COMM_WORLD , &rank ) ;
	#else
	//Ifonly MPI isavailable/present, and OpenMP is not .
	MPI_Init(&argc, &argv) ;
	#endif
	// If only MPI is available present, independently of OpenMP
	MPI_Comm_size ( MPI_COMM_WORLD , &size ) ;
	MPI_Comm_rank ( MPI_COMM_WORLD , &rank ) ;
	#else
	//If neither MPI nor OpenMP is available/ present
	rank = 0;
	size = 1;
	#endif
	if (rank == 0) {
		char *inputLine = NULL; size_t lineLength = 0;

		// Read in first line of input
		getline(&inputLine, &lineLength, stdin);
		sscanf(inputLine, "%d", &amountOfRuns);

		stop = (int*) calloc(amountOfRuns, sizeof(int));
		start = (int*) calloc(amountOfRuns, sizeof(int));
		numThreads = (int*) calloc(amountOfRuns, sizeof(int));
		sum = (int*) calloc(amountOfRuns, sizeof(int));
		if (size != 0)
			total_sum = (int*) calloc(amountOfRuns*size, sizeof(int));

		int tot_threads, current_start, current_stop;
		for (int i = 0; i < amountOfRuns; ++i){

			// Read in each line of input that follows after first line
			free(inputLine); lineLength = 0; inputLine = NULL;
			ssize_t readChars = getline(&inputLine, &lineLength, stdin);

			// If there exists at least two matches (2x %d)...
			if (sscanf(inputLine, "%d %d %d", &current_start, &current_stop, &tot_threads) >= 2){
				if(current_start < 0 || current_stop < 0){
					current_start = 0, current_stop = 0;
				}
				stop[i] = current_stop;
				start[i] = current_start;
				numThreads[i] = tot_threads;
			}
		}
	}

	#ifdef HAVE_MPI
	MPI_Bcast(&amountOfRuns, 1,MPI_INT, 0, MPI_COMM_WORLD);
	if (rank != 0) {
		stop = (int*) calloc(amountOfRuns, sizeof(int));
		start = (int*) calloc(amountOfRuns, sizeof(int));
		sum = (int*) calloc(amountOfRuns, sizeof(int));
		numThreads = (int*) calloc(amountOfRuns, sizeof(int));
	}
	MPI_Bcast(stop, amountOfRuns,MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(start, amountOfRuns,MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(numThreads, amountOfRuns,MPI_INT, 0, MPI_COMM_WORLD);

	// divide loop

	for (int i=0; i < amountOfRuns; i++){
		int cnt = numThreads[i];
		int cstop = stop[i];
		int cstart = start[i];
		int tmp_sum = 0;
		int mystart = (cstop / size) * rank;
		int myend;
		if (cstop % size > rank){
			mystart += rank;
			myend = mystart + (cstop / size) + 1;
		} else {
			mystart += cstop % size;
			myend = mystart + (cstop / size);
		}
		if (mystart == 0) mystart++;

		sum[i] = run(cstart, cstop, cnt, mystart, myend);

	}

	MPI_Gather(sum, amountOfRuns, MPI_INT, total_sum, amountOfRuns, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		int *ns;
		ns = (int*)calloc(amountOfRuns, sizeof(int));
		for (int i=0;i<amountOfRuns;i++) {
			for (int j=0;j<size;j++) {
				ns[i] += total_sum[i + amountOfRuns*j];
			}
		}
		for (int u=0;u<amountOfRuns;u++) {
			printf("%d\n", ns[u]);
		}
	}
	#else
	for (int i=0; i < amountOfRuns; i++){
		int s = run(start[i], stop[i], numThreads[i], 0, stop[i]);
		printf("%d\n", s);
	}
	#endif


	/*
	*	Remember to only print 1 (one) sum per start/stop.
	*	In other words, a total of <amountOfRuns> sums/printfs.
	*/
	#ifdef HAVE_MPI
	MPI_Finalize();
	#endif
	return 0;
}
