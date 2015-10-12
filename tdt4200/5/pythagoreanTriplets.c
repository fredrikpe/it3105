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
	if (start == 0) start++;

	int sum = 0;
	int a, b, c;
	for (int n=1; n<stop; n++) {
		for (int m=n+1; m<stop; m+=2) {
			if (2*m*n >= start && m*m - n*n >= start) {
				if (m*m + n*n < stop && coPrime(m, n)) {
					sum++;
				}
			}
		}
	}

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
	printf("Sum: %d\n", sum);
}


int main(int argc, char **argv) {
	char *inputLine = NULL; size_t lineLength = 0;
	int *start, *stop, *numThreads, amountOfRuns = 0;

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

	for (int i = 0; i < amountOfRuns; ++i){
		run(start[i], stop[i], numThreads[i]);
	}

	/*
	*	Remember to only print 1 (one) sum per start/stop.
	*	In other words, a total of <amountOfRuns> sums/printfs.
	*/

	return 0;
}
