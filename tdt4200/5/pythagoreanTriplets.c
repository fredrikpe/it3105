#include <stdio.h> // for stdin
#include <stdlib.h>
#include <unistd.h> // for ssize_t

#ifdef HAVE_MPI
#include <mpi.h>
#endif

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

int coPrime (int a, int b) { // Assumes a, b > 0
	return (a<b) ? coPrime(b,a) : !(a % b) ? (b==1) : coPrime (b, a%b);
}

void run(int start, int stop, int numThreads) {
	if (numThreads == 0) numThreads = 1;
	int sum = 0;
	if (start == 0) start++;

	int a, b, c;
	#pragma omp parallel for num_threads(numThreads) reduction(+:sum)
	for (int n=1; n<stop; n++) {
		for (int m=n+1; m<stop; m+=2) {
			//if (2*m*n >= start && m*m - n*n >= start) {
			if (coPrime(m, n) && m*m + n*n >= start && m*m + n*n < stop) {
				sum++;
			}
		}
	}
	//#pragma omp parallel num_threads(numThreads)
	//{
		//int tid = omp_get_thread_num();
	  //printf("Hello World from thread = %d\n", tid);
	//}


	/*
	for (int a=start; a<stop-1; a++) {
		for (int b=a+1; b<stop; b++) {
			int c = b + 1;
			while (c * c < a * a + b * b) {
				c++;
			}
			if (c * c == a * a + b * b && c <= stop) {
				if (coPrime(a, b) && coPrime(b, c) && coPrime(a,c)) {
					printf("%d, %d, %d. ", a, b, c);
					sum++;
				}
			}
		}
	}
	*/
	printf("%d\n", sum);
}


int main(int argc, char **argv) {

	int rank, size;
	int *start, *stop, *numThreads;
	int amountOfRuns = 0;


	#ifdef HAVE_MPI
	#ifdef HAVE_OPENMP
	// If both MPI and OpenMP is available / present
	MPI_Init_thread(&argc, &argv , MPI_THREAD_FUNNELED , aquired );
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
  printf("CPU%d %d ~ %d\n",rank,mystart,myend);
  // compute PI
  for (int i = mystart ; i < myend ; i++){
		printf("Running run #: %d from process #: %d\n", i, rank);
		run(start[i], stop[i], numThreads[i]);
	}
	#else
	for (int i=0; i < amountOfRuns; i++){
		run(start[i], stop[i], numThreads[i]);
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
