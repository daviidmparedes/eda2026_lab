#include "poscode/sort.hpp"
#include "poscode/utils.hpp"

// ===========================================================================
// AQUI TRABAJAS TU.
//
// Hay tres funciones por completar y estan en orden: cada una usa la anterior.
//
//   1. counting_sort            -> Parte 2a de la prueba
//   2. counting_sort_by_digit   -> la pieza que necesita radixsort
//   3. radix_sort               -> Parte 2b de la prueba
//
// quick_sort y merge_sort (mas abajo) YA ESTAN RESUELTOS: son los dos
// algoritmos vistos en clase contra los que hay que comparar, y son los mismos
// del laboratorio 2 adaptados a Poscode.
//
// Mientras una funcion este sin implementar, ./test la marca como FALLA en la
// tabla, pero el resto sigue corriendo igual.
// ===========================================================================

// ---------------------------------------------------------------------------
// 1) PARTE 2a - COUNTING SORT SOBRE DIGITOS
// ---------------------------------------------------------------------------
//
// Ordena un arreglo de n enteros que solo toma valores en [0, 9], en O(n).
//
// La idea: como hay solo M = 10 valores posibles, no hace falta comparar
// elementos entre si. Basta con contar cuantas veces aparece cada valor y
// despues escribirlos en orden.
//
// Fijate que NO hay ningun ciclo anidado sobre n: son tres recorridos de A y
// dos de los 10 contadores. Por eso T(n) = 2M + 2n = O(n).
//
void counting_sort(int *A, size_t n){
	// TODO: implementar.
}

// ---------------------------------------------------------------------------
// 2) PARTE 2b - COUNTING SORT POR UN DIGITO
// ---------------------------------------------------------------------------
//
// Lo mismo que counting_sort, pero sobre codigos: a que grupo va cada codigo
// lo decide UN solo digito, el de la posicion d (d = 0 es el mas
// significativo, el de mas a la izquierda).
//
// Para sacar el digito d de un codigo como un entero 0..9:
//
//     int b = A[i].getValue(d) - '0';
//
// CUIDADO: el algoritmo tiene que ser ESTABLE. Dos codigos con el mismo digito
// d deben quedar en el MISMO orden relativo que tenian antes.
//
// Sin estabilidad radix_sort no funciona, y el error es silencioso: el arreglo
// igual queda ordenado por ese digito. ./test trae una prueba especifica.
//
void counting_sort_by_digit(Poscode *A, size_t n, size_t d){
	// TODO: implementar.
}

// ---------------------------------------------------------------------------
// 3) PARTE 2b - RADIXSORT
// ---------------------------------------------------------------------------
//
// Ordena los n codigos en O(n), usando counting_sort_by_digit.
//
// El largo de un codigo se obtiene asi:
//
//     size_t p = A[0].getData().size();   // 5 en este dataset
//
// Son p pasadas de O(n), o sea T(n) = O(np). Como p = 5 es una constante, en
// la practica es O(n).
//
void radix_sort(Poscode *A, size_t n){
	// TODO: implementar.
}

// ===========================================================================
// ALGORITMOS DE COMPARACION
//
// Son los mismos del laboratorio 2, con dos cambios: ordenan Poscode en vez de
// float, y comparan con getData() (como todos los codigos tienen el mismo
// largo, el orden alfabetico de los strings es el orden numerico).
// ===========================================================================

static void swapCodes(Poscode *A, int i, int j){
	Poscode aux = A[i];
	A[i] = A[j];
	A[j] = aux;
}

// Particion de quicksort. El pivote se elige al azar.
static int split_qs(Poscode *A, int i, int j){
	int p = getRandomInt(i, j);

	while (i < j){
		while (i < p && A[i].getData() <= A[p].getData()){
			i = i + 1;
		}
		while (j > p && A[j].getData() >= A[p].getData()){
			j = j - 1;
		}
		swapCodes(A, i, j);
		// El pivote se movio con el swap: hay que seguirle la pista.
		if (i == p){
			p = j;
		}
		else if (j == p){
			p = i;
		}
	}
	return p;
}

static void quickSortRec(Poscode *A, int i, int j){
	if (i < j){
		int p = split_qs(A, i, j);
		quickSortRec(A, i, p - 1);
		quickSortRec(A, p + 1, j);
	}
}

void quick_sort(Poscode *A, size_t n){
	if (n < 2){
		return;
	}
	quickSortRec(A, 0, static_cast<int>(n) - 1);
}

// Mezcla A[i..k] con A[k+1..j], ambos ya ordenados. El arreglo auxiliar se
// reserva dentro de merge, o sea en cada una de las ~n llamadas: es la version
// vista en clases. El costo de esas reservas se comenta en el README.
static void merge(Poscode *A, int i, int j, int k){
	Poscode *Aaux = new Poscode[j - i + 1];
	int q = 0;
	int p1 = i;
	int p2 = k + 1;
	while (p1 <= k && p2 <= j){
		if (A[p1].getData() <= A[p2].getData()){
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
	delete[] Aaux;
}

static void mergeSortRec(Poscode *A, int i, int j){
	int k = (i + j) / 2;
	if (i < j){
		mergeSortRec(A, i, k);
		mergeSortRec(A, k + 1, j);
		merge(A, i, j, k);
	}
}

void merge_sort(Poscode *A, size_t n){
	if (n < 2){
		return;
	}
	mergeSortRec(A, 0, static_cast<int>(n) - 1);
}
