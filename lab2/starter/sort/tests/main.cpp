#include "sort/sort.hpp"
#include "sort/utils.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <ctime>

// ====================================================================
// PASO 1 - Elige aca el algoritmo basico que vas a implementar.
//
//   1 = selectionSort  (Cap. 2.7.1, Alg. 9)   <- ya viene resuelto en eda_cpp
//   2 = insertionSort  (Cap. 2.7.2, Alg. 10)
//   3 = bubbleSort     (extra, NO esta en el libro)
//   0 = los tres a la vez (solo si implementaste los tres)
//
// Los algoritmos que no elijas no se llaman ni aparecen en las tablas, asi
// que puedes dejarlos sin implementar sin romper nada.
// ====================================================================
static const int ALGORITMO_BASICO = 2;

// Semilla del generador aleatorio. Igual que en eda_cpp/sort/tests/test.cpp:
// cada corrida usa datos distintos y quicksort elige pivotes distintos.
// Si quieres repetir exactamente la misma corrida, cambia la linea por
// std::srand(42) (o cualquier constante).
static void initRandomState(){
	std::srand(std::time(nullptr));
}

// Puntero a cualquier algoritmo de ordenamiento: todos tienen la misma firma.
typedef void (*SortFn)(float*, int);

// mergeSort, quickSort y quickSortMiddle estan sobrecargados con (A,i,j) y
// (A,n). Estos envoltorios fijan la version (A,n) para poder guardarlas en
// un puntero a funcion.
static void mergeSortN(float* A, int n){ sort::mergeSort(A, n); }
static void quickSortN(float* A, int n){ sort::quickSort(A, n); }
static void quickSortMiddleN(float* A, int n){ sort::quickSortMiddle(A, n); }

struct Algo {
	const char* name;  // nombre de la columna en la tabla y en el csv
	SortFn fn;
	int maxN;          // 0 = sin tope; si no, no se mide para n mayores
};

// Tamanos del barrido. Se DUPLICAN en cada fila: en un grafico log-log la
// pendiente de la curva es entonces el exponente del costo. (El test oficial
// de eda_cpp usa sort::linspace, que reparte los tamanos de forma lineal;
// aca conviene la escala geometrica para leer el exponente directamente.)
static const int NS[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000};
static const int N_COUNT = sizeof(NS) / sizeof(NS[0]);

// Los algoritmos cuadraticos no se miden sobre este tamano: a n = 64000
// burbuja tarda del orden de 10 s por repeticion y solo repetiria la misma
// informacion que ya entregan las filas anteriores.
static const int CUADRATICO_MAX = 32000;

static Algo ALGOS[6];
static int ALGO_COUNT = 0;

static void addAlgo(const char* name, SortFn fn, int maxN){
	ALGOS[ALGO_COUNT].name = name;
	ALGOS[ALGO_COUNT].fn = fn;
	ALGOS[ALGO_COUNT].maxN = maxN;
	ALGO_COUNT++;
}

static void buildAlgoList(){
	if (ALGORITMO_BASICO == 0 || ALGORITMO_BASICO == 1){
		addAlgo("seleccion", sort::selectionSort, CUADRATICO_MAX);
	}
	if (ALGORITMO_BASICO == 0 || ALGORITMO_BASICO == 2){
		addAlgo("insercion", sort::insertionSort, CUADRATICO_MAX);
	}
	if (ALGORITMO_BASICO == 0 || ALGORITMO_BASICO == 3){
		addAlgo("burbuja", sort::bubbleSort, CUADRATICO_MAX);
	}
	addAlgo("mergesort", mergeSortN, 0);
	addAlgo("quick_azar", quickSortN, 0);
	addAlgo("quick_centro", quickSortMiddleN, 0);
}

// Mismo nombre que el helper de eda_cpp/sort/tests/test.cpp, pero devolviendo
// milisegundos como double: el original hace duration_cast a milisegundos
// enteros, y a n = 1000 los algoritmos rapidos tardan menos de 1 ms, con lo
// que toda la fila saldria como 0.
static double getElapsedTime(std::chrono::time_point<std::chrono::high_resolution_clock> t1,
		std::chrono::time_point<std::chrono::high_resolution_clock> t2){
	return std::chrono::duration<double, std::milli>(t2 - t1).count();
}

// Cuantas veces se repite cada medicion. En vez de fijar un numero, se repite
// hasta acumular PRESUPUESTO_MS. Asi los algoritmos rapidos (que a n chico
// tardan microsegundos y son pura fluctuacion) se repiten muchas veces, y los
// lentos, que ya tardan segundos por corrida, se repiten el minimo.
// Esto importa sobre todo para mergesort: el Alg. 11 reserva un arreglo
// auxiliar en CADA llamada a merge, o sea unas n veces por ordenamiento, y el
// costo de esas reservas depende del estado del heap. Sin repetir lo suficiente,
// la fila de mergesort salta de forma erratica entre una corrida y otra.
static const double PRESUPUESTO_MS = 300.0;
static const int MIN_REPS = 3;
static const int MAX_REPS = 25;

// Mide fn sobre copias de base y retorna el MENOR tiempo en ms.
// Se usa el minimo y no el promedio porque el ruido externo (otros procesos,
// el planificador del sistema operativo, el asignador de memoria) solo puede
// sumar tiempo, nunca restarlo.
// Retorna -1 si el algoritmo no dejo el arreglo ordenado: el tiempo de una
// implementacion incorrecta no significa nada, asi que no se reporta.
static double measureMs(SortFn fn, float* base, int n){
	double best = -1.0;
	double total = 0.0;
	for (int r = 0; r < MAX_REPS && (r < MIN_REPS || total < PRESUPUESTO_MS); r++){
		float* A = sort::copyArray(base, n);
		std::chrono::time_point<std::chrono::high_resolution_clock> t1 =
			std::chrono::high_resolution_clock::now();
		fn(A, n);
		std::chrono::time_point<std::chrono::high_resolution_clock> t2 =
			std::chrono::high_resolution_clock::now();
		double ms = getElapsedTime(t1, t2);
		bool ok = sort::isSorted(A, n);
		sort::deleteArray(A);
		if (!ok){
			return -1.0;
		}
		total = total + ms;
		if (best < 0.0 || ms < best){
			best = ms;
		}
	}
	return best;
}

// Corre el barrido completo: imprime la tabla en pantalla y la escribe en csvPath.
static void runSweep(const char* title, const char* csvPath, bool sortedInput){
	std::cout << std::endl << title << std::endl;

	std::ofstream out(csvPath);
	out << "n";
	std::cout << std::setw(9) << "n";
	for (int a = 0; a < ALGO_COUNT; a++){
		out << "," << ALGOS[a].name;
		std::cout << std::setw(14) << ALGOS[a].name;
	}
	out << std::endl;
	std::cout << std::endl;

	std::cout << std::fixed << std::setprecision(3);
	for (int k = 0; k < N_COUNT; k++){
		int n = NS[k];
		float* base = sortedInput ? sort::createSortedArray(n) : sort::createRandomArray(n);

		out << n;
		std::cout << std::setw(9) << n;
		for (int a = 0; a < ALGO_COUNT; a++){
			if (ALGOS[a].maxN > 0 && n > ALGOS[a].maxN){
				// Celda vacia en el csv: el script de graficos la omite.
				out << ",";
				std::cout << std::setw(14) << "-";
			}
			else{
				double ms = measureMs(ALGOS[a].fn, base, n);
				if (ms < 0.0){
					// Todavia sin implementar, o con un bug.
					out << ",";
					std::cout << std::setw(14) << "FALLA";
				}
				else{
					out << "," << ms;
					std::cout << std::setw(14) << ms;
				}
			}
		}
		out << std::endl;
		std::cout << std::endl;

		sort::deleteArray(base);
	}
	out.close();
	std::cout << "Tabla escrita en " << csvPath << std::endl;
}

// Verificacion sobre un arreglo chico, con los valores a la vista.
static void runDemo(){
	std::cout << "Verificacion sobre un arreglo pequeno" << std::endl;
	int n = 10;
	float* original = sort::createRandomIntArray(n, 0, 100);
	std::cout << "  " << std::setw(14) << "original" << ": ";
	sort::printArray(original, n);
	for (int a = 0; a < ALGO_COUNT; a++){
		float* A = sort::copyArray(original, n);
		ALGOS[a].fn(A, n);
		std::cout << "  " << std::setw(14) << ALGOS[a].name << ": ";
		sort::printArray(A, n);
		if (!sort::isSorted(A, n)){
			std::cout << "  " << std::setw(14) << " " << "  ^^^ no quedo ordenado" << std::endl;
		}
		sort::deleteArray(A);
	}
	sort::deleteArray(original);
}

int main(int nargs, char** vargs){
	initRandomState();
	buildAlgoList();

	runDemo();

	// Barrido con entrada aleatoria: el caso promedio.
	runSweep("Tiempos en ms - entrada aleatoria (menor de varias repeticiones)",
	         "times_random.csv", false);

	// Barrido con entrada ya ordenada: muestra el mejor caso de los algoritmos
	// basicos y verifica que ninguna de las dos elecciones de pivote se degrada.
	runSweep("Tiempos en ms - entrada ya ordenada",
	         "times_sorted.csv", true);

	return 0;
}
