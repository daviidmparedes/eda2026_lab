#include "reorder/reorder.hpp"
#include <iostream>

void printArray(int* A, int n) {
	for (int i = 0; i < n; i++) {
		std::cout << A[i] << " ";
	}
	std::cout << std::endl;
}

void runCase(const char* name, int* A, int n) {
	std::cout << name << std::endl;
	std::cout << "  antes:   ";
	printArray(A, n);
	reorder::reorder_neg_pos(A, n);
	std::cout << "  despues: ";
	printArray(A, n);
}

int main(int nargs, char** vargs) {
	int A1[] = {-2, 11, -1, 3, -3, -2};
	runCase("caso mixto", A1, 6);

	int A2[] = {-1, -2, -3, -4};
	runCase("todo negativo", A2, 4);

	int A3[] = {5, 2, 8, 1};
	runCase("todo positivo", A3, 4);

	int A4[] = {3};
	runCase("un elemento", A4, 1);

	int* A5 = nullptr;
	runCase("arreglo vacio", A5, 0);

	return 0;
}
