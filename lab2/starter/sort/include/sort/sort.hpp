#ifndef SORT_SORT_HPP
#define SORT_SORT_HPP

namespace sort{

	// Todos los algoritmos ordenan A[0..n-1] de menor a mayor, in-place.
	// Se usa float* para mantener la misma interfaz que eda_cpp/sort.

	// ---- Cap. 2.7.1 - Ordenacion por Seleccion (Alg. 9) ----
	// O(n^2) siempre: no tiene mejor ni peor caso.
	void selectionSort(float* A, int n);

	// ---- Cap. 2.7.2 - Ordenacion por Insercion (Alg. 10) ----
	// O(n^2) en el peor caso (entrada invertida), O(n) si A ya viene ordenado.
	void insertionSort(float* A, int n);

	// ---- Extra: NO aparece en el libro ----
	// Se incluye solo como tercera opcion a elegir en el Paso 1 del laboratorio.
	// O(n^2) en el peor caso, O(n) si A ya viene ordenado (por el corte temprano).
	void bubbleSort(float* A, int n);

	// ---- Cap. 2.7.3 - MergeSort (Alg. 11 y 12) ----
	// Ojo con el orden de los parametros de merge: es (A, i, j, k), tal como lo
	// llama el Alg. 12. Las dos sublistas ordenadas son A[i..k] y A[k+1..j].
	void merge(float* A, int i, int j, int k);
	void mergeSort(float* A, int i, int j);
	void mergeSort(float* A, int n);

	// ---- Cap. 2.7.4 - QuickSort (Alg. 13 y 14) ----
	// split_qs es el Alg. 13 tal cual: elige el pivote AL AZAR.
	// split_qs_middle es la variante de este laboratorio: mismo algoritmo de
	// division, pero con el pivote fijo en la posicion CENTRAL del subarreglo.
	int split_qs(float* A, int i, int j);
	int split_qs_middle(float* A, int i, int j);

	void quickSort(float* A, int i, int j);        // pivote al azar (libro)
	void quickSort(float* A, int n);
	void quickSortMiddle(float* A, int i, int j);  // pivote al centro (variante)
	void quickSortMiddle(float* A, int n);

}

#endif
