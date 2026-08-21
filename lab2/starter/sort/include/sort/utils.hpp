#ifndef SORT_UTILS_HPP
#define SORT_UTILS_HPP

namespace sort{

	// --- Identicas a las de eda_cpp/sort/include/sort/utils.hpp ---
	float* createArray(int n);
	float* createRandomArray(int n);
	float* createRandomIntArray(int n, int minVal = 0, int maxVal = 100);
	void deleteArray(float* A);
	void printArray(float* A, int n);
	void setAll(float* A, int n, float value);
	void swap(float* A, int i, int j);
	int getRandomInt(int min, int max);
	int* linspace(int max, int n_parts);

	// --- Agregadas para este laboratorio ---
	// Arreglo ya ordenado 0, 1, 2, ..., n-1. Sirve para medir el mejor caso.
	float* createSortedArray(int n);
	// Copia independiente de A. Cada algoritmo debe medirse sobre los MISMOS
	// datos, asi que se ordena siempre una copia y nunca el arreglo original.
	float* copyArray(float* A, int n);
	bool isSorted(float* A, int n);

}

#endif
