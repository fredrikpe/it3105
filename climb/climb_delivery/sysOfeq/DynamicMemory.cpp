#include <algorithm>
#include <iostream>
#include "DynamicMemory.h"
#include "utilities.h"

using namespace std;

void fillInFibonacciNumbers(int result[], int length) {
	result[0] = 0;

	if (length == 1) {
		return;
	}

	result[1] = 1;

	if (length <= 2) {
		return;
	}

	for(int i = 2; i < length; i++) {
		result[i] = result[i-1] + result[i-2];
	}
}

void createFibonacci() {
	cout << "Hvor mange tall skal generereres fra Fibonacci-følgen?" << endl;
	int length;
	cin >> length;

	int *result = new int[length];
	fillInFibonacciNumbers(result, length);

	std::cout << "Fibonacci-følgen:" << std::endl;
	printArray(result, length);

	delete [] result;
}
