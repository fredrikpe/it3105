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

int coPrime (int a, int b) { // Assumes a, b > 0
	return (a<b) ? coPrime(b,a) : !(a % b) ? (b==1) : coPrime (b, a%b);
}

int run(int start, int stop, int numThreads) {
	if (numThreads == 0) numThreads = 1;
	int sum = 0;
	if (start == 0) start++; // Unnecessary

	int a, b, c;
	//#pragma omp parallel for num_threads(numThreads) reduction(+:sum)
	for (int n=1; n<stop; n++) {
		for (int m=n+1; m<stop; m+=2) {
			//if (2*m*n >= start && m*m - n*n >= start)
			if (coPrime(m, n) && m*m + n*n >= start && m*m + n*n < stop) {
				sum++;
			}
		}
	}
	/*
	//#pragma omp parallel for reduction(+:sum)
	for (int a=3; a<stop-1; a++) {
		for (int b=a+1; b<stop; b+=2) {
			if (coPrime(a, b)) {
				int c = b + 1;
				while (c < start || c * c < a * a + b * b) {
					c++;
				}
				if (c * c == a * a + b * b && c < stop) {
					sum++;
				}
			}
		}
	}*/
	
	
	/*#pragma omp parallel for num_threads(numThreads) reduction(+:sum)
	for (int a=3; a<stop-1; a++) {
		for (int b=a+1; b<stop; b+=2) {
			if (coPrime(a, b)) {
				float c = sqrt(a * a + b * b);
				if (ceilf(c) == c && c >= start && c < stop) {
					sum++;
				}
			}
		}
	}*/
	return sum;
}


int main(int argc, char **argv) {
	
	printf("omp   %d\n", omp_get_max_threads() );
	int *buffer;
	int rank, size;
	int *start, *stop, *numThreads;
	int amountOfRuns = 0;
	int aquired;

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
		buffer = (int*) calloc(amountOfRuns*2, sizeof(int));

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
		numThreads = (int*) calloc(amountOfRuns, sizeof(int));
	}
	MPI_Bcast(stop, amountOfRuns,MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(start, amountOfRuns,MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(numThreads, amountOfRuns,MPI_INT, 0, MPI_COMM_WORLD);
	// divide loop

  int mystart = (amountOfRuns / size) * rank;
	int myend;
  if (amountOfRuns % size > rank){
    mystart += rank;
    myend = mystart + (amountOfRuns / size) + 1;
  }else{
    mystart += amountOfRuns % size;
    myend = mystart + (amountOfRuns / size);
  }
  //printf("CPU%d %d ~ %d\n",rank,mystart,myend);
  // compute PI
	int *s = (int*)calloc(amountOfRuns/size + 1, sizeof(int));
	for (int i = 0; i < amountOfRuns/size + 1; i++) {
		s[i] = -1;
	}
  for (int i = mystart ; i < myend ; i++){
		int t = run(start[i], stop[i], numThreads[i]);
		s[i-mystart] = t;
		//printf("Running run #: %d from process #: %d, sum: %d\n", i, rank, s);
	}

	MPI_Gather(s, amountOfRuns/size + 1, MPI_INT, buffer, amountOfRuns/size + 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		int c = 0;
		for (int i=0; i< amountOfRuns*2;i++) {
			if (buffer[i] != -1 && c < amountOfRuns) {
				printf("%d\n", buffer[i]);
				c++;
			}
		}
	}
	#else
	for (int i=0; i < amountOfRuns; i++){
		int s = run(start[i], stop[i], numThreads[i]);
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
