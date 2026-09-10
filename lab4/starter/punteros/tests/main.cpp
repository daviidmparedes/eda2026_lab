#include "punteros/indireccion.hpp"
#include "punteros/memoria.hpp"
#include "punteros/arreglos.hpp"
#include "punteros/funciones.hpp"
#include "punteros/proteccion.hpp"
#include "punteros/alcance.hpp"

#include <iomanip>
#include <iostream>

// Cada funcion del laboratorio tiene una prueba que retorna true si quedo
// correcta. Las pruebas estan escritas para no caerse cuando una funcion
// todavia no esta implementada: si algo entrega nullptr, se reporta FALLA y se
// sigue con la siguiente.

static int nOk = 0;
static int nFalla = 0;

static void check(const char *nombre, bool ok){
	std::cout << "  " << std::left << std::setw(18) << nombre
	          << (ok ? "[ok]" : "[FALLA]") << std::endl;
	if (ok){ nOk++; }
	else{ nFalla++; }
}

static void tema(const char *titulo){
	std::cout << std::endl << titulo << std::endl;
}

// ---------------------------------------------------------------- tema 1 ---

static bool testValorFinal(){
	int n = 42;
	int *p = &n;
	int **pp = &p;
	int ***ppp = &pp;
	return valorFinal(ppp) == 42;
}

static bool testReapuntar(){
	int a = 5;
	int b = 9;
	int *p = &a;
	reapuntar(&p, &b, 100);
	// p tiene que haber quedado apuntando a b, b vale 100 y a no se toco.
	return p == &b && b == 100 && a == 5;
}

// ---------------------------------------------------------------- tema 2 ---

static bool testReservarLleno(){
	int *v = reservarLleno(5, 7);
	if (v == nullptr){
		return false;
	}
	bool ok = true;
	for (int i = 0; i < 5; i++){
		if (v[i] != 7){ ok = false; }
	}
	delete[] v;
	// Con un tamano no valido tiene que entregar nullptr.
	return ok && reservarLleno(0, 7) == nullptr;
}

static bool testLiberarYAnular(){
	int *v = new int[3];
	liberarYAnular(v);
	// Si quedo en nullptr, libero y anulo como corresponde. Si no quedo nulo,
	// no se libera aca: la funcion podria haber liberado ya y seria un doble
	// delete.
	return v == nullptr;
}

// ---------------------------------------------------------------- tema 3 ---

static bool testSumArray(){
	const int a[5] = {1, 2, 3, 4, 5};
	return sumArray(a, 5) == 15 && sumArray(a, 1) == 1 && sumArray(a, 0) == 0;
}

static bool testReverseArray(){
	int impar[5] = {1, 2, 3, 4, 5};
	const int esperadoImpar[5] = {5, 4, 3, 2, 1};
	reverseArray(impar, 5);
	for (int i = 0; i < 5; i++){
		if (impar[i] != esperadoImpar[i]){ return false; }
	}
	// Tambien tiene que servir con una cantidad par de elementos.
	int par[4] = {1, 2, 3, 4};
	const int esperadoPar[4] = {4, 3, 2, 1};
	reverseArray(par, 4);
	for (int i = 0; i < 4; i++){
		if (par[i] != esperadoPar[i]){ return false; }
	}
	return true;
}

// ---------------------------------------------------------------- tema 4 ---

static bool testStatsArray(){
	const int a[6] = {4, -2, 9, 9, 0, 1};
	int min = 0;
	int max = 0;
	double prom = 0.0;
	statsArray(a, 6, min, max, prom);
	// La suma es 21 y son 6 elementos, asi que el promedio es 3.5 exacto.
	if (min != -2 || max != 9){ return false; }
	if (prom < 3.4999 || prom > 3.5001){ return false; }

	// Con n no valido no debe tocar ninguna de las tres salidas.
	int min2 = 77;
	int max2 = 88;
	double prom2 = 99.0;
	statsArray(a, 0, min2, max2, prom2);
	return min2 == 77 && max2 == 88 && prom2 == 99.0;
}

static bool testCrearArreglo(){
	int *p = nullptr;
	crearArreglo(p, 4);
	if (p == nullptr){
		return false;
	}
	bool ok = true;
	for (int i = 0; i < 4; i++){
		if (p[i] != 0){ ok = false; }
	}
	delete[] p;
	return ok;
}

// ---------------------------------------------------------------- tema 5 ---

static bool testContarMenores(){
	const int a[6] = {1, 50, 3, 99, 7, 100};
	// Menores que 10: 1, 3 y 7.
	return contarMenores(a, 6, 10) == 3
	    && contarMenores(a, 6, 0) == 0
	    && contarMenores(a, 6, 200) == 6;
}

static bool testCopiarArreglo(){
	const int origen[4] = {2, 4, 6, 8};
	int destino[4] = {0, 0, 0, 0};
	copiarArreglo(origen, destino, 4);
	for (int i = 0; i < 4; i++){
		if (destino[i] != origen[i]){ return false; }
	}
	return true;
}

// ---------------------------------------------------------------- tema 6 ---

static bool testConstruirRango(){
	int *r = construirRango(10);
	if (r == nullptr){
		return false;
	}
	bool ok = (r[0] == 0 && r[1] == 5 && r[2] == 10);
	delete[] r;
	return ok;
}

static bool testConcatArray(){
	const int a[2] = {1, 2};
	const int b[3] = {3, 4, 5};
	int *r = concatArray(a, 2, b, 3);
	if (r == nullptr){
		return false;
	}
	const int esperado[5] = {1, 2, 3, 4, 5};
	bool ok = true;
	for (int i = 0; i < 5; i++){
		if (r[i] != esperado[i]){ ok = false; }
	}
	delete[] r;
	return ok;
}

// ---------------------------------------------------------------------------

int main(int nargs, char **vargs){
	std::cout << "Laboratorio 4 - Punteros, arreglos y funciones" << std::endl;

	tema("Tema 1 - Punteros e indireccion");
	check("valorFinal", testValorFinal());
	check("reapuntar", testReapuntar());

	tema("Tema 2 - Memoria dinamica");
	check("reservarLleno", testReservarLleno());
	check("liberarYAnular", testLiberarYAnular());

	tema("Tema 3 - Punteros y arreglos");
	check("sumArray", testSumArray());
	check("reverseArray", testReverseArray());

	tema("Tema 4 - Punteros y funciones");
	check("statsArray", testStatsArray());
	check("crearArreglo", testCrearArreglo());

	tema("Tema 5 - Proteccion de los datos");
	check("contarMenores", testContarMenores());
	check("copiarArreglo", testCopiarArreglo());

	tema("Tema 6 - Alcance y tiempo de vida");
	check("construirRango", testConstruirRango());
	check("concatArray", testConcatArray());

	std::cout << std::endl << "Resumen: " << nOk << " de " << (nOk + nFalla)
	          << " correctas" << std::endl;
	if (nFalla > 0){
		std::cout << "Quedan " << nFalla << " por implementar." << std::endl;
	}
	return 0;
}
