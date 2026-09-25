#include "poscode/sort.hpp"
#include "poscode/utils.hpp"

// ===========================================================================
// PARTE 2a - COUNTING SORT SOBRE DIGITOS   (libro, Cap. 2.8.1, Alg. 15)
// ===========================================================================
//
// NOTA PARA EL EQUIPO DOCENTE: en starter/ estas funciones se llaman
// counting_sort / counting_sort_by_digit a proposito, y ahi no se menciona ni
// el libro ni el nombre bucketsort. Parte de la prueba es que el estudiante
// note por su cuenta que es el Alg. 15 del curso. Este archivo es el unico
// que difiere del starter: todo lo demas es identico en los dos arboles.
//
// El arreglo solo contiene valores en [0, 9], o sea M = 10 buckets. El libro
// llena cada bucket con las POSICIONES de los elementos que le corresponden y
// despues recorre los buckets en orden para escribir el resultado en Aaux.
//
// Aca los buckets no se guardan como listas: basta con contar cuantos
// elementos cae en cada uno y sumar esos totales para saber en que posicion de
// Aaux empieza cada bucket. Es exactamente el mismo recorrido del Alg. 15
// (los elementos del bucket b salen juntos y en el orden en que se
// encontraron), pero sin reservar una lista por bucket.
//
// Tiempo: 2M + 2n con M = 10 (Eq. 2.17). Como M no depende de n, es O(n).
// ---------------------------------------------------------------------------
void counting_sort(int *A, size_t n){
	if (n < 2){
		return;
	}

	// 1) Cuantos elementos van a cada bucket.
	size_t count[10];
	for (int b = 0; b < 10; b++){
		count[b] = 0;
	}
	for (size_t i = 0; i < n; i++){
		count[A[i]] = count[A[i]] + 1;
	}

	// 2) Donde empieza cada bucket dentro del arreglo de salida.
	size_t start[10];
	size_t acc = 0;
	for (int b = 0; b < 10; b++){
		start[b] = acc;
		acc = acc + count[b];
	}

	// 3) Cada elemento se copia a la posicion que le toca dentro de su bucket.
	int *Aaux = new int[n];
	for (size_t i = 0; i < n; i++){
		int b = A[i];
		Aaux[start[b]] = A[i];
		start[b] = start[b] + 1;
	}

	// 4) El resultado vuelve a A.
	for (size_t i = 0; i < n; i++){
		A[i] = Aaux[i];
	}
	delete[] Aaux;
}

// ===========================================================================
// PARTE 2b - RADIXSORT SOBRE LOS CODIGOS   (libro, Cap. 2.8.2)
// ===========================================================================

// Mismo counting sort de arriba, pero el bucket de cada codigo lo decide UN digito
// (el de la posicion d) en vez del valor completo.
//
// El recorrido del paso 3 va de izquierda a derecha y dentro de cada bucket las
// posiciones se ocupan en ese mismo orden, asi que dos codigos con el mismo
// digito d conservan su orden relativo: el algoritmo es estable, que es lo que
// radix_sort necesita.
void counting_sort_by_digit(Poscode *A, size_t n, size_t d){
	if (n < 2){
		return;
	}

	size_t count[10];
	for (int b = 0; b < 10; b++){
		count[b] = 0;
	}
	for (size_t i = 0; i < n; i++){
		int b = A[i].getValue(d) - '0';
		count[b] = count[b] + 1;
	}

	size_t start[10];
	size_t acc = 0;
	for (int b = 0; b < 10; b++){
		start[b] = acc;
		acc = acc + count[b];
	}

	Poscode *Aaux = new Poscode[n];
	for (size_t i = 0; i < n; i++){
		int b = A[i].getValue(d) - '0';
		Aaux[start[b]] = A[i];
		start[b] = start[b] + 1;
	}

	for (size_t i = 0; i < n; i++){
		A[i] = Aaux[i];
	}
	delete[] Aaux;
}

// Ordena de a un digito, empezando por el MENOS significativo (el de mas a la
// derecha, indice p-1) y terminando en el mas significativo (indice 0).
//
// Funciona porque cada pasada es estable: cuando se ordena por el digito d, los
// codigos que empatan en ese digito quedan en el orden que les dejo la pasada
// anterior, o sea ordenados por los digitos menos significativos.
//
// Tiempo: p pasadas de O(n) cada una, T(n) = O(np). Con p = 5 fijo, es O(n).
void radix_sort(Poscode *A, size_t n){
	if (n < 2){
		return;
	}
	size_t p = A[0].getData().size();
	for (size_t d = p; d-- > 0; ){
		counting_sort_by_digit(A, n, d);
	}
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

// Particion de quicksort, libro Cap. 2.7.5, Alg. 13. El pivote se elige al azar.
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

// Libro Cap. 2.7.5, Alg. 14.
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

// Libro Cap. 2.7.4, Alg. 11: mezcla A[i..k] con A[k+1..j], ambos ya ordenados.
// Igual que en el libro, el arreglo auxiliar se reserva dentro de merge, o sea
// en cada una de las ~n llamadas. Se deja asi para no alejarse del algoritmo
// del curso; el costo de esas reservas se comenta en el README.
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

// Libro Cap. 2.7.4, Alg. 12.
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
