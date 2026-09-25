#include "poscode/poscode.hpp"
#include "poscode/sort.hpp"
#include "poscode/utils.hpp"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

// Rutas que define CMakeLists.txt. DATA_FILE apunta al dataset binario
// (lab3/data/codes_1M.bin) y OUTPUT_DIR a la carpeta python/ del ejercicio,
// para que el csv quede junto al script que lo grafica.
#ifndef DATA_FILE
#define DATA_FILE "../../data/codes_1M.bin"
#endif
#ifndef OUTPUT_DIR
#define OUTPUT_DIR "."
#endif

static std::string outputPath(const char *fileName){
	return std::string(OUTPUT_DIR) + "/" + fileName;
}

// Semilla del generador aleatorio. Igual que en eda_cpp/sort/tests/test.cpp:
// afecta solo la eleccion de pivote de quick_sort, no los datos (el dataset es
// fijo y viene del archivo). Para repetir exactamente una corrida, cambiar por
// std::srand(42).
static void initRandomState(){
	std::srand(std::time(nullptr));
}

// Los tres algoritmos que se comparan tienen la misma firma.
typedef void (*SortFn)(Poscode*, size_t);

struct Algo {
	const char *name;   // nombre de la columna en la tabla y en el csv
	SortFn fn;
	size_t maxN;        // 0 = sin tope; si no, no se mide para n mayores
};

// Valores de n pedidos en la Prueba 1.
static const size_t NS[] = {1000, 10000, 50000, 100000, 500000, 1000000};
static const int N_COUNT = sizeof(NS) / sizeof(NS[0]);

static Algo ALGOS[3];
static int ALGO_COUNT = 0;

static void addAlgo(const char *name, SortFn fn, size_t maxN){
	ALGOS[ALGO_COUNT].name = name;
	ALGOS[ALGO_COUNT].fn = fn;
	ALGOS[ALGO_COUNT].maxN = maxN;
	ALGO_COUNT++;
}

static void buildAlgoList(){
	addAlgo("quicksort", quick_sort, 0);
	addAlgo("mergesort", merge_sort, 0);
	addAlgo("radixsort", radix_sort, 0);
}

// Mismo helper que eda_cpp/sort/tests/test.cpp, pero en milisegundos como
// double: el original hace duration_cast a enteros y a n = 1000 radixsort
// tarda menos de 1 ms, con lo que la fila saldria como 0.
static double getElapsedTime(std::chrono::time_point<std::chrono::high_resolution_clock> t1,
		std::chrono::time_point<std::chrono::high_resolution_clock> t2){
	return std::chrono::duration<double, std::milli>(t2 - t1).count();
}

// Se repite cada medicion hasta acumular PRESUPUESTO_MS. Asi los casos rapidos
// (que a n chico son pura fluctuacion) se repiten muchas veces y los lentos,
// que ya tardan segundos, se repiten el minimo.
static const double PRESUPUESTO_MS = 400.0;
static const int MIN_REPS = 3;
static const int MAX_REPS = 25;

// Mide fn sobre copias de base y retorna el MENOR tiempo en ms. Se usa el
// minimo y no el promedio porque el ruido externo (otros procesos, el
// planificador, el asignador de memoria) solo puede sumar tiempo, nunca restarlo.
// Retorna -1 si el algoritmo no dejo el arreglo ordenado: el tiempo de una
// implementacion incorrecta no significa nada, asi que no se reporta.
static double measureMs(SortFn fn, Poscode *base, size_t n){
	double best = -1.0;
	double total = 0.0;
	for (int r = 0; r < MAX_REPS && (r < MIN_REPS || total < PRESUPUESTO_MS); r++){
		Poscode *A = copyCodes(base, n);
		std::chrono::time_point<std::chrono::high_resolution_clock> t1 =
			std::chrono::high_resolution_clock::now();
		fn(A, n);
		std::chrono::time_point<std::chrono::high_resolution_clock> t2 =
			std::chrono::high_resolution_clock::now();
		double ms = getElapsedTime(t1, t2);
		bool ok = isSorted(A, n);
		deleteCodes(A);
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

// ---------------------------------------------------------------------------
// Verificaciones previas al barrido de tiempos
// ---------------------------------------------------------------------------

// Parte 2a: counting sort sobre un arreglo de digitos.
static void testCountingSort(){
	std::cout << std::endl << "Parte 2a - counting sort sobre digitos (0-9)" << std::endl;
	const int n = 16;
	int A[n] = {5, 3, 9, 0, 3, 7, 1, 9, 4, 0, 8, 2, 6, 3, 5, 1};
	int esperado[n] = {0, 0, 1, 1, 2, 3, 3, 3, 4, 5, 5, 6, 7, 8, 9, 9};

	std::cout << "  entrada  : ";
	for (int i = 0; i < n; i++){ std::cout << A[i] << " "; }
	std::cout << std::endl;

	counting_sort(A, n);

	std::cout << "  ordenado : ";
	for (int i = 0; i < n; i++){ std::cout << A[i] << " "; }
	std::cout << std::endl;

	bool ok = true;
	for (int i = 0; i < n; i++){
		if (A[i] != esperado[i]){ ok = false; }
	}
	std::cout << "  resultado: " << (ok ? "[ok]" : "[FALLA]") << std::endl;
}

// Parte 2b: counting_sort_by_digit tiene que ser ESTABLE. Es el error mas comun
// al programar radixsort, y ademas es silencioso: el arreglo igual queda
// "ordenado por ese digito", pero radixsort deja de funcionar.
static void testEstabilidad(){
	std::cout << std::endl << "Parte 2b - counting_sort_by_digit debe ser estable" << std::endl;
	const size_t n = 8;
	const char *entrada[n]  = {"10001", "20002", "10003", "30004",
	                           "20005", "10006", "30007", "20008"};
	// Ordenando SOLO por el primer digito (d = 0), los codigos que empiezan
	// igual tienen que conservar el orden en que venian.
	const char *esperado[n] = {"10001", "10003", "10006", "20002",
	                           "20005", "20008", "30004", "30007"};

	Poscode *A = new Poscode[n];
	for (size_t i = 0; i < n; i++){
		A[i] = Poscode(entrada[i]);
	}

	std::cout << "  entrada  : ";
	printCodes(A, n);

	counting_sort_by_digit(A, n, 0);

	std::cout << "  por d=0  : ";
	printCodes(A, n);

	bool ok = true;
	for (size_t i = 0; i < n; i++){
		if (A[i].getData() != esperado[i]){ ok = false; }
	}
	std::cout << "  resultado: " << (ok ? "[ok] es estable" : "[FALLA] no es estable") << std::endl;
	deleteCodes(A);
}

// Los tres algoritmos deben producir EXACTAMENTE el mismo arreglo. isSorted()
// por si solo no basta: un radixsort con un contador mal calculado puede dejar
// algo ordenado pero con codigos repetidos o perdidos.
static void testCoinciden(Poscode *base, size_t n){
	std::cout << std::endl << "Los tres algoritmos deben dar el mismo resultado (n = " << n << ")" << std::endl;
	Poscode *ref = copyCodes(base, n);
	quick_sort(ref, n);

	for (int a = 0; a < ALGO_COUNT; a++){
		Poscode *A = copyCodes(base, n);
		ALGOS[a].fn(A, n);
		bool ok = true;
		for (size_t i = 0; i < n; i++){
			if (A[i].getData() != ref[i].getData()){ ok = false; }
		}
		std::cout << "  " << std::setw(10) << ALGOS[a].name << ": "
		          << (ok ? "[ok]" : "[FALLA]") << std::endl;
		deleteCodes(A);
	}
	deleteCodes(ref);
}

// Verificacion sobre un arreglo chico, con los codigos a la vista.
static void runDemo(Poscode *base){
	std::cout << std::endl << "Verificacion sobre 12 codigos del dataset" << std::endl;
	const size_t n = 12;
	std::cout << "  " << std::setw(10) << "original" << ": ";
	printCodes(base, n);
	for (int a = 0; a < ALGO_COUNT; a++){
		Poscode *A = copyCodes(base, n);
		ALGOS[a].fn(A, n);
		std::cout << "  " << std::setw(10) << ALGOS[a].name << ": ";
		printCodes(A, n);
		if (!isSorted(A, n)){
			std::cout << "  " << std::setw(10) << " " << "  ^^^ no quedo ordenado" << std::endl;
		}
		deleteCodes(A);
	}
}

// ---------------------------------------------------------------------------
// Barrido de tiempos
// ---------------------------------------------------------------------------
static void runSweep(const std::string &dataFile, const char *csvName){
	std::cout << std::endl
	          << "Tiempos en ms (menor de varias repeticiones)" << std::endl;

	std::string csvPath = outputPath(csvName);
	std::ofstream out(csvPath.c_str());
	out << "n";
	std::cout << std::setw(10) << "n";
	for (int a = 0; a < ALGO_COUNT; a++){
		out << "," << ALGOS[a].name;
		std::cout << std::setw(14) << ALGOS[a].name;
	}
	out << std::endl;
	std::cout << std::endl;

	std::cout << std::fixed << std::setprecision(3);
	for (int k = 0; k < N_COUNT; k++){
		size_t n = NS[k];
		Poscode *base = readCodes(dataFile, n);
		if (base == nullptr){
			return;
		}

		out << n;
		std::cout << std::setw(10) << n;
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

		deleteCodes(base);
	}
	out.close();
	std::cout << "Tabla escrita en " << csvPath << std::endl;
}

int main(int nargs, char **vargs){
	initRandomState();
	buildAlgoList();

	std::string dataFile(DATA_FILE);
	size_t disponibles = countCodes(dataFile);
	if (disponibles == 0){
		std::cerr << "No pude leer el dataset en " << dataFile << std::endl;
		std::cerr << "Generalo con: python3 lab3/data/generate_codes.py" << std::endl;
		return 1;
	}
	std::cout << "Dataset: " << dataFile << std::endl;
	std::cout << "         " << disponibles << " codigos disponibles" << std::endl;

	testCountingSort();
	testEstabilidad();

	Poscode *muestra = readCodes(dataFile, 100000);
	if (muestra == nullptr){
		return 1;
	}
	runDemo(muestra);
	testCoinciden(muestra, 100000);
	deleteCodes(muestra);

	runSweep(dataFile, "times_codes.csv");

	return 0;
}
