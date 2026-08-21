#include <iostream>
#include "sort/sort.hpp"
#include "sort/utils.hpp"

namespace sort{

	// ==================================================================
	// Cap. 2.7.1 - Ordenacion por Seleccion (Alg. 9)
	// En cada posicion i se busca el menor de los que faltan por ordenar
	// (de i hasta el final) y se intercambia con el de la posicion i.
	// Misma implementacion que eda_cpp/sort/src/sort.cpp.
	// ==================================================================
	void selectionSort(float* A, int n){
		int smallest = 0;
		int i = 0;
		int j = 0;
		for (i = 0; i < n - 1; i++){
			smallest = i;
			for (j = i + 1; j < n; j++){
				if (A[j] < A[smallest]){
					smallest = j;
				}
			}
			swap(A,i,smallest);
		}
	}

	// ==================================================================
	// Cap. 2.7.2 - Ordenacion por Insercion (Alg. 10)
	// Estrategia de ordenar barajas: se toma A[i] y se lo ubica en su
	// posicion correcta hacia la izquierda, corriendo los mayores.
	// ==================================================================
	void insertionSort(float* A, int n){
		for (int i = 1; i < n; i++){
			float elem = A[i];
			int j = i - 1;
			while (j >= 0 && elem < A[j]){
				A[j + 1] = A[j];
				j = j - 1;
			}
			A[j + 1] = elem;
		}
	}

	// ==================================================================
	// Extra: NO aparece en el libro.
	// Recorre el arreglo comparando pares vecinos e intercambiandolos si
	// estan en desorden. En cada pasada el mayor "burbujea" hasta el final,
	// asi que despues de la pasada k las ultimas k posiciones ya estan listas.
	// El flag swapped corta apenas una pasada no hace ningun intercambio,
	// lo que le da un mejor caso O(n) sobre entradas ya ordenadas.
	// ==================================================================
	void bubbleSort(float* A, int n){
		for (int i = 0; i < n - 1; i++){
			bool swapped = false;
			for (int j = 0; j < n - 1 - i; j++){
				if (A[j] > A[j + 1]){
					swap(A, j, j + 1);
					swapped = true;
				}
			}
			if (!swapped){
				return;
			}
		}
	}

	// ==================================================================
	// Cap. 2.7.3 - MergeSort (Alg. 11 y 12)
	// ==================================================================

	// Alg. 11. Mezcla las sublistas YA ordenadas A[i..k] y A[k+1..j].
	// Recorre ambas una sola vez con dos indices p1 y p2, copiando en cada
	// paso el menor de los dos a un arreglo auxiliar. Costo O(j - i + 1).
	void merge(float* A, int i, int j, int k){
		float* Aaux = createArray(j - i + 1);
		int q = 0;
		int p1 = i;
		int p2 = k + 1;
		while (p1 <= k && p2 <= j){
			if (A[p1] <= A[p2]){
				Aaux[q] = A[p1];
				p1 = p1 + 1;
			}
			else{
				Aaux[q] = A[p2];
				p2 = p2 + 1;
			}
			q = q + 1;
		}
		while (p1 <= k){
			Aaux[q] = A[p1];
			p1 = p1 + 1;
			q = q + 1;
		}
		while (p2 <= j){
			Aaux[q] = A[p2];
			p2 = p2 + 1;
			q = q + 1;
		}
		for (q = 0; q < j - i + 1; q++){
			A[i + q] = Aaux[q];
		}
		deleteArray(Aaux);
	}

	// Alg. 12. Divide en mitades, ordena cada una y mezcla.
	void mergeSort(float* A, int i, int j){
		int k = (i + j) / 2;
		if (i < j){
			mergeSort(A, i, k);
			mergeSort(A, k + 1, j);
			merge(A, i, j, k);
		}
	}

	void mergeSort(float* A, int n){
		mergeSort(A, 0, n - 1);
	}

	// ==================================================================
	// Cap. 2.7.4 - QuickSort (Alg. 13 y 14)
	// ==================================================================

	// Alg. 13 (split_qs) con la posicion del pivote recibida como parametro.
	// Las dos variantes del laboratorio comparten TODA esta funcion y se
	// diferencian solo en como calculan p, mas abajo.
	//
	// Avanza i mientras encuentre elementos <= pivote y retrocede j mientras
	// encuentre elementos >= pivote. Cuando ambos se detienen, intercambia.
	// Si el que se movio fue el propio pivote, actualiza p a su nueva posicion.
	static int split_at(float* A, int i, int j, int p){
		while (i < j){

			while ( i < p && A[i] <= A[p]){
				i = i + 1;
			}

			while ( j > p && A[j] >= A[p]){
				j = j - 1;
			}

			swap(A, i, j);

			if (i == p){
				p = j;
			}
			else if (j == p){
				p = i;
			}
		}
		return p;
	}

	// Version del libro: el pivote se elige al azar dentro de [i, j].
	int split_qs(float* A, int i, int j){
		return split_at(A, i, j, getRandomInt(i, j));
	}

	// Variante del laboratorio: el pivote es siempre el del centro de [i, j].
	int split_qs_middle(float* A, int i, int j){
		return split_at(A, i, j, i + (j - i) / 2);
	}

	// Alg. 14. Divide con split y ordena recursivamente cada sublista.
	// El pivote queda en su posicion definitiva k, por eso NO entra en
	// ninguna de las dos llamadas recursivas.
	void quickSort(float* A, int i, int j){
		if (i < j){
			int k = split_qs(A, i, j);
			quickSort(A, i, k - 1);
			quickSort(A, k + 1, j);
		}
	}

	void quickSort(float* A, int n){
		quickSort(A, 0, n - 1);
	}

	void quickSortMiddle(float* A, int i, int j){
		if (i < j){
			int k = split_qs_middle(A, i, j);
			quickSortMiddle(A, i, k - 1);
			quickSortMiddle(A, k + 1, j);
		}
	}

	void quickSortMiddle(float* A, int n){
		quickSortMiddle(A, 0, n - 1);
	}

}
