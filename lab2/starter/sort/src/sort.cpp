#include <iostream>
#include "sort/sort.hpp"
#include "sort/utils.hpp"

namespace sort{

	// ==================================================================
	// PASO 1 - Elige UNO de los tres algoritmos basicos que siguen e
	// implementalo. Los otros dos puedes dejarlos como estan: si en
	// tests/main.cpp pones el ALGORITMO_BASICO que corresponde, los que no
	// elegiste no se llaman nunca y no aparecen en las tablas.
	// ==================================================================

	// ------------------------------------------------------------------
	// Opcion 1 - Ordenacion por Seleccion (Cap. 2.7.1, Alg. 9)
	// En cada posicion i se busca el MENOR de los que faltan por ordenar
	// (los que van de i hasta el final) y se intercambia con A[i].
	// Se llega hasta el penultimo: cuando ese queda ordenado, el ultimo
	// tambien lo esta.
	// ------------------------------------------------------------------
	void selectionSort(float* A, int n){
		// TODO (opcion 1): dos ciclos anidados.
		//   - el de afuera recorre i de 0 a n-2
		//   - guarda smallest = i
		//   - el de adentro recorre j de i+1 a n-1 y actualiza smallest
		//     cada vez que A[j] < A[smallest]
		//   - al terminar el ciclo interno: swap(A, i, smallest)
		// Nota: este algoritmo ya esta resuelto en el repositorio del curso,
		// en eda_cpp/sort/src/sort.cpp, por si quieres compararlo despues.
	}

	// ------------------------------------------------------------------
	// Opcion 2 - Ordenacion por Insercion (Cap. 2.7.2, Alg. 10)
	// Estrategia de ordenar barajas: se toma A[i] (desde el segundo) y se
	// lo ubica en su posicion correcta hacia la izquierda, corriendo un
	// lugar a la derecha a todos los que sean mayores que el.
	// ------------------------------------------------------------------
	void insertionSort(float* A, int n){
		// TODO (opcion 2): para cada i de 1 a n-1:
		//   - guarda elem = A[i] y parte con j = i - 1
		//   - mientras j >= 0 y elem < A[j]: copia A[j] en A[j+1] y baja j
		//   - al salir del while: A[j+1] = elem
		// Ojo: hay que guardar elem ANTES del while, porque el ciclo va
		// sobrescribiendo A[i].
	}

	// ------------------------------------------------------------------
	// Opcion 3 - Ordenacion Burbuja (NO esta en el libro)
	// Se recorre el arreglo comparando pares vecinos e intercambiandolos
	// si estan en desorden. En cada pasada el mayor de los que quedan
	// "burbujea" hasta el final, asi que despues de la pasada i las
	// ultimas i posiciones ya estan en su lugar.
	// ------------------------------------------------------------------
	void bubbleSort(float* A, int n){
		// TODO (opcion 3): dos ciclos anidados.
		//   - el de afuera recorre i de 0 a n-2
		//   - el de adentro recorre j de 0 a n-2-i y hace
		//     swap(A, j, j+1) cada vez que A[j] > A[j+1]
		// Extra: lleva un bool swapped que parta en false y se ponga en
		// true con cada intercambio. Si una pasada completa termina sin
		// ningun intercambio, el arreglo ya esta ordenado y puedes cortar
		// con un return. Eso le da un mejor caso O(n), igual que insercion.
	}

	// ==================================================================
	// PASO 2 - MergeSort (Cap. 2.7.3, Alg. 11 y 12)
	// ==================================================================

	// Alg. 11. Mezcla las sublistas YA ordenadas A[i..k] y A[k+1..j].
	// Debe costar O(j - i + 1): cada elemento se toca una sola vez.
	// Ojo con el orden de los parametros: es (A, i, j, k), tal como lo
	// llama el Alg. 12. El punto de corte k va al final, no al medio.
	void merge(float* A, int i, int j, int k){
		// TODO: reserva un auxiliar del tamano del tramo:
		//     float* Aaux = createArray(j - i + 1);
		//   Luego avanza dos indices en paralelo, p1 desde i (sublista
		//   izquierda, termina en k) y p2 desde k+1 (sublista derecha,
		//   termina en j), copiando en cada paso el MENOR de A[p1] y A[p2]
		//   a Aaux[q].
		//   Cuando una de las dos sublistas se agota, copia lo que quede
		//   de la otra (son dos while sueltos, uno para cada caso).
		//   Al final copia Aaux de vuelta: A[i + q] = Aaux[q].
		//   No olvides deleteArray(Aaux) o el programa va a botar memoria
		//   en cada una de las ~n llamadas.
		// Sugerencia: usar A[p1] <= A[p2] (y no <) mantiene el orden
		// relativo de los elementos iguales, es decir, hace estable al
		// mergesort.
	}

	// Alg. 12. Divide en mitades, ordena cada una y mezcla.
	void mergeSort(float* A, int i, int j){
		// TODO:
		//   - k = (i + j) / 2 es el punto de division
		//   - si i < j: ordena A[i..k] y A[k+1..j] recursivamente con
		//     mergeSort, y despues mezclalos con merge(A, i, j, k)
		//   - el caso base (i >= j) es un tramo de 0 o 1 elementos: ya
		//     esta ordenado, no hay nada que hacer
	}

	void mergeSort(float* A, int n){
		mergeSort(A, 0, n - 1);
	}

	// ==================================================================
	// PASO 3 - QuickSort (Cap. 2.7.4, Alg. 13 y 14)
	// ==================================================================

	// Alg. 13 (split_qs), con la posicion del pivote como parametro extra.
	// Las dos variantes del laboratorio comparten TODA esta funcion: lo
	// unico que cambia entre ellas es como se calcula p, mas abajo. Asi,
	// cualquier diferencia de tiempo que midas viene solo de la eleccion
	// del pivote y no de dos implementaciones distintas.
	//
	// Al terminar, todo lo que quede a la izquierda de la posicion
	// retornada debe ser <= al pivote, y todo lo de la derecha >=.
	static int split_at(float* A, int i, int j, int p){
		// TODO: mientras i < j:
		//   1. avanza i mientras i < p y A[i] <= A[p]
		//      (busca por la izquierda un elemento que NO deberia estar ahi)
		//   2. retrocede j mientras j > p y A[j] >= A[p]
		//      (busca por la derecha un elemento que NO deberia estar ahi)
		//   3. swap(A, i, j)
		//   4. si el que se movio fue el propio pivote, hay que seguirle la
		//      pista: si i == p entonces p = j; si no, si j == p entonces p = i.
		// Al salir del while, retorna p.
		return p;
	}

	// Version del libro: el pivote se elige AL AZAR dentro de [i, j].
	int split_qs(float* A, int i, int j){
		// TODO: elige la posicion del pivote al azar y llama a split_at.
		// getRandomInt(min, max) esta en sort/utils.hpp y devuelve un entero
		// entre min y max, ambos incluidos.
		return split_at(A, i, j, i);
	}

	// Variante del laboratorio: el pivote es siempre el del CENTRO de [i, j].
	int split_qs_middle(float* A, int i, int j){
		// TODO: calcula la posicion central del tramo y llama a split_at.
		// Ojo: es el centro de [i, j], no el centro del arreglo completo.
		return split_at(A, i, j, i);
	}

	// Alg. 14. Divide con split y ordena recursivamente cada sublista.
	void quickSort(float* A, int i, int j){
		// TODO: si i < j:
		//   - k = split_qs(A, i, j)
		//   - ordena recursivamente A[i..k-1] y A[k+1..j]
		// El pivote queda en su posicion DEFINITIVA k, por eso no entra en
		// ninguna de las dos llamadas. Esa exclusion es justamente lo que
		// hace que la recursion termine: si incluyes k, no avanza nunca y
		// el programa cae con segmentation fault.
	}

	void quickSort(float* A, int n){
		quickSort(A, 0, n - 1);
	}

	// Igual que quickSort, pero llamando a split_qs_middle.
	void quickSortMiddle(float* A, int i, int j){
		// TODO: lo mismo que quickSort, pero usando split_qs_middle.
	}

	void quickSortMiddle(float* A, int n){
		quickSortMiddle(A, 0, n - 1);
	}

}
