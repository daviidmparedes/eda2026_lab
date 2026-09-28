#include "busqueda/llist.hpp"
#include "busqueda/stack.hpp"
#include "busqueda/queue.hpp"
#include "busqueda/camino.hpp"
#include "busqueda/jarras.hpp"
#include "busqueda/laberinto.hpp"

#include <iomanip>
#include <iostream>

// Cada parte tiene sus pruebas: [ok] si el resultado es el esperado, [FALLA]
// si no. Estan escritas para no caerse cuando una funcion todavia no esta
// implementada: si algo entrega nullptr, se reporta FALLA y se sigue.
//
// Ojo: lo que las pruebas NO pueden atajar es una busqueda que no termina.
// Si ./test se queda pegado, revisa el arreglo visitado y la condicion del
// while.

static int nOk = 0;
static int nFalla = 0;

static void check(const char *nombre, bool ok){
	std::cout << "  " << std::left << std::setw(38) << nombre
	          << (ok ? "[ok]" : "[FALLA]") << std::endl;
	if (ok){ nOk++; }
	else{ nFalla++; }
}

static void parte(const char *titulo){
	std::cout << std::endl << "==== " << titulo << std::endl;
}

// ---------------------------------------------------------------------------
// Utilidades para revisar listas
// ---------------------------------------------------------------------------

static int largo(LList *L){
	int k = 0;
	if (L != nullptr){
		Node *p = L->getHead();
		while (p != nullptr){
			k++;
			p = p->getNext();
		}
	}
	return k;
}

// true si L tiene exactamente los n ids de esperado, en ese orden.
static bool listaEs(LList *L, const int *esperado, int n){
	if (L == nullptr || largo(L) != n){
		return false;
	}
	Node *p = L->getHead();
	for (int i = 0; i < n; i++){
		if (p->getId() != esperado[i]){
			return false;
		}
		p = p->getNext();
	}
	return true;
}

// Revisa el resultado de una busqueda: que coincida con lo esperado, que no
// haya camino (esperado == nullptr), y libera la lista.
static bool revisarYBorrar(LList *L, const int *esperado, int n){
	bool ok;
	if (esperado == nullptr){
		ok = (L == nullptr);
	}
	else{
		ok = listaEs(L, esperado, n);
	}
	delete L;
	return ok;
}

// ===========================================================================
// PARTE 1 - El mapa de la clase
// ===========================================================================

static const int A = 0, B = 1, C = 2, D = 3, E = 4, F = 5;

// El mismo mapa del lunes, insertado en el mismo orden. Como insert() es al
// inicio, cada lista se recorre al reves de como se inserto.
static LList *crearMapa(){
	LList *adj = new LList[6];
	adj[A].insert(B);  adj[A].insert(C);    // A: C, B
	adj[B].insert(C);  adj[B].insert(D);    // B: D, C
	adj[C].insert(E);  adj[C].insert(F);    // C: F, E
	adj[D].insert(C);  adj[D].insert(E);    // D: E, C
	adj[E].insert(F);                       // E: F
	return adj;                             // F: sin salidas
}

static void imprimirMapa(LList *adj, int n){
	std::cout << "  El mapa (cada lista en su orden de recorrido):" << std::endl;
	for (int x = 0; x < n; x++){
		std::cout << "    " << static_cast<char>('A' + x) << ": ";
		Node *p = adj[x].getHead();
		if (p == nullptr){
			std::cout << "-";
		}
		while (p != nullptr){
			std::cout << static_cast<char>('A' + p->getId()) << " ";
			p = p->getNext();
		}
		std::cout << std::endl;
	}
}

static void imprimirLetras(LList *L){
	if (L == nullptr){
		std::cout << "no hay camino";
		return;
	}
	Node *p = L->getHead();
	while (p != nullptr){
		std::cout << static_cast<char>('A' + p->getId()) << " ";
		p = p->getNext();
	}
}

static void testParte1(){
	parte("Parte 1 - Buscar un camino en el mapa de la clase");
	LList *adj = crearMapa();
	imprimirMapa(adj, 6);

	std::cout << std::endl << "  1a - existeCamino" << std::endl;
	// Las pruebas de "no hay camino" solo cuentan si la funcion ya encuentra
	// los que si hay: una funcion vacia que siempre responde false acertaria
	// esas pruebas sin hacer nada. Lo mismo vale en las partes 2 y 3.
	bool anda = existeCamino(adj, 6, A, E);
	check("existeCamino A -> E", anda);
	check("existeCamino C -> B (no hay)", anda && !existeCamino(adj, 6, C, B));
	check("existeCamino D -> F", existeCamino(adj, 6, D, F));
	check("existeCamino A -> A", existeCamino(adj, 6, A, A));
	check("existeCamino E -> A (no hay)", anda && !existeCamino(adj, 6, E, A));

	std::cout << std::endl << "  Trazas (compara con las de la pizarra):" << std::endl;
	std::cout << "  A -> E" << std::endl;
	existeCamino(adj, 6, A, E, true);
	std::cout << "  C -> B" << std::endl;
	existeCamino(adj, 6, C, B, true);
	std::cout << "  D -> F (el ticket de salida)" << std::endl;
	existeCamino(adj, 6, D, F, true);

	std::cout << std::endl << "  1b - buscarCamino (pila)" << std::endl;
	const int ae[] = {A, B, C, E};
	const int df[] = {D, C, E, F};
	const int af[] = {A, B, C, E, F};
	const int aa[] = {A};
	anda = revisarYBorrar(buscarCamino(adj, 6, A, E), ae, 4);
	check("buscarCamino A -> E = A B C E", anda);
	check("buscarCamino D -> F = D C E F", revisarYBorrar(buscarCamino(adj, 6, D, F), df, 4));
	check("buscarCamino A -> F = A B C E F", revisarYBorrar(buscarCamino(adj, 6, A, F), af, 5));
	check("buscarCamino A -> A = A", revisarYBorrar(buscarCamino(adj, 6, A, A), aa, 1));
	check("buscarCamino C -> B = nullptr", anda && revisarYBorrar(buscarCamino(adj, 6, C, B), nullptr, 0));

	std::cout << std::endl << "  1c - Queue y buscarCaminoCola" << std::endl;
	{
		// Tres fichas en orden 1, 2, 3: tienen que salir en ese mismo orden.
		Queue Q;
		Q.push(new DataNode(1));
		Q.push(new DataNode(2));
		Q.push(new DataNode(3));
		bool ok = !Q.isEmpty() && Q.front() != nullptr && Q.front()->getData()->getId() == 1;
		if (ok){ Q.pop(); ok = Q.front() != nullptr && Q.front()->getData()->getId() == 2; }
		if (ok){ Q.pop(); ok = Q.front() != nullptr && Q.front()->getData()->getId() == 3; }
		if (ok){ Q.pop(); ok = Q.isEmpty(); }
		check("Queue: sale en orden FIFO", ok);
	}
	{
		// Despues de vaciarla, la cola tiene que seguir funcionando. Falla si
		// pop no devuelve tail a nullptr cuando la cola queda vacia.
		Queue Q;
		Q.push(new DataNode(7));
		Q.pop();
		Q.push(new DataNode(8));
		bool ok = !Q.isEmpty() && Q.front() != nullptr && Q.front()->getData()->getId() == 8;
		check("Queue: se vacia y se vuelve a usar", ok);
	}
	const int aeCola[] = {A, C, E};
	const int dfCola[] = {D, E, F};
	const int afCola[] = {A, C, F};
	anda = revisarYBorrar(buscarCaminoCola(adj, 6, A, E), aeCola, 3);
	check("buscarCaminoCola A -> E = A C E", anda);
	check("buscarCaminoCola D -> F = D E F", revisarYBorrar(buscarCaminoCola(adj, 6, D, F), dfCola, 3));
	check("buscarCaminoCola A -> F = A C F", revisarYBorrar(buscarCaminoCola(adj, 6, A, F), afCola, 3));
	check("buscarCaminoCola C -> B = nullptr", anda && revisarYBorrar(buscarCaminoCola(adj, 6, C, B), nullptr, 0));

	std::cout << std::endl << "  Pila contra cola:" << std::endl;
	const int pares[3][2] = {{A, E}, {D, F}, {A, F}};
	for (int k = 0; k < 3; k++){
		int o = pares[k][0];
		int d = pares[k][1];
		LList *pila = buscarCamino(adj, 6, o, d);
		LList *cola = buscarCaminoCola(adj, 6, o, d);
		std::cout << "    " << static_cast<char>('A' + o) << " -> " << static_cast<char>('A' + d)
		          << "   pila: ";
		imprimirLetras(pila);
		std::cout << "  cola: ";
		imprimirLetras(cola);
		std::cout << std::endl;
		delete pila;
		delete cola;
	}

	delete[] adj;
}

// ===========================================================================
// PARTE 2 - Las jarras
// ===========================================================================

// Estas funciones NO usan las del estudiante: sirven para revisarlas.
static int idJ(int a, int b){ return a * (CAP_B + 1) + b; }
static int aDe(int id){ return id / (CAP_B + 1); }
static int bDe(int id){ return id % (CAP_B + 1); }

// Que operacion lleva de un estado al otro (solo para imprimir).
static const char *operacion(int desde, int hasta){
	int a = aDe(desde), b = bDe(desde);
	int na = aDe(hasta), nb = bDe(hasta);
	if (na == CAP_A && nb == b){ return "llenar A"; }
	if (na == a && nb == CAP_B){ return "llenar B"; }
	if (na == 0 && nb == b){ return "vaciar A"; }
	if (na == a && nb == 0){ return "vaciar B"; }
	if (na + nb == a + b && na < a){ return "A -> B"; }
	if (na + nb == a + b && na > a){ return "B -> A"; }
	return "???";
}

static void imprimirJarras(LList *L){
	if (L == nullptr){
		std::cout << "      no hay solucion" << std::endl;
		return;
	}
	Node *p = L->getHead();
	int paso = 0;
	int anterior = -1;
	while (p != nullptr){
		int id = p->getId();
		std::cout << "      " << std::right << std::setw(2) << paso << "  (" << aDe(id) << "," << bDe(id) << ")";
		if (anterior >= 0){
			std::cout << "   " << operacion(anterior, id);
		}
		std::cout << std::endl;
		anterior = id;
		paso++;
		p = p->getNext();
	}
}

static bool sucesoresSon(int a, int b, const int *esperado, int n){
	int suc[MAX_SUC_JARRAS];
	for (int i = 0; i < MAX_SUC_JARRAS; i++){
		suc[i] = -1;
	}
	int k = sucesoresJarras(idJ(a, b), suc);
	if (k != n){
		return false;
	}
	for (int i = 0; i < n; i++){
		if (suc[i] != esperado[i]){
			return false;
		}
	}
	return true;
}

static void testParte2(){
	parte("Parte 2 - Las jarras (A de 3 L, B de 5 L)");

	std::cout << "  2a - El modelo" << std::endl;
	check("idJarras(2, 5) = 17", idJarras(2, 5) == 17);
	check("idJarras(0, 0) = 0 y (3, 5) = 23", idJarras(0, 0) == 0 && idJarras(3, 5) == 23);
	check("litrosA(17) = 2, litrosB(17) = 5", litrosA(17) == 2 && litrosB(17) == 5);

	const int de00[] = {idJ(3, 0), idJ(0, 5)};
	const int de25[] = {idJ(3, 5), idJ(0, 5), idJ(2, 0), idJ(3, 4)};
	const int de10[] = {idJ(3, 0), idJ(1, 5), idJ(0, 0), idJ(0, 1)};
	check("sucesores de (0,0): (3,0) (0,5)", sucesoresSon(0, 0, de00, 2));
	check("sucesores de (2,5): 4, uno es meta", sucesoresSon(2, 5, de25, 4));
	check("sucesores de (1,0): 4", sucesoresSon(1, 0, de10, 4));

	std::cout << std::endl << "  2b - resolverJarras (pila)" << std::endl;
	const int pila4[] = {idJ(0, 0), idJ(0, 5), idJ(3, 2), idJ(3, 0), idJ(0, 3), idJ(3, 3),
	                     idJ(1, 5), idJ(1, 0), idJ(0, 1), idJ(3, 1), idJ(0, 4)};
	bool anda = revisarYBorrar(resolverJarras(4), pila4, 11);
	check("resolverJarras(4): 10 pasos", anda);
	check("resolverJarras(6) = nullptr", anda && revisarYBorrar(resolverJarras(6), nullptr, 0));

	std::cout << std::endl << "  2c - resolverJarrasCola" << std::endl;
	const int cola4[] = {idJ(0, 0), idJ(0, 5), idJ(3, 2), idJ(0, 2), idJ(2, 0), idJ(2, 5), idJ(3, 4)};
	anda = revisarYBorrar(resolverJarrasCola(4), cola4, 7);
	check("resolverJarrasCola(4): 6 pasos", anda);
	check("resolverJarrasCola(6) = nullptr", anda && revisarYBorrar(resolverJarrasCola(6), nullptr, 0));

	std::cout << std::endl << "  Dejar 4 litros en B:" << std::endl;
	LList *pila = resolverJarras(4);
	std::cout << "    con pila:" << std::endl;
	imprimirJarras(pila);
	delete pila;
	LList *cola = resolverJarrasCola(4);
	std::cout << "    con cola:" << std::endl;
	imprimirJarras(cola);
	delete cola;
}

// ===========================================================================
// PARTE 3 - El laberinto
// ===========================================================================

// Revisa que L sea un camino de verdad: parte en (f0, c0), termina en
// (f1, c1), pasa solo por celdas libres y avanza de a un paso. No revisa CUAL
// camino es, porque eso depende del orden en que se apilan los vecinos.
static bool caminoValido(Laberinto &lab, LList *L, int f0, int c0, int f1, int c1){
	if (L == nullptr || L->getHead() == nullptr){
		return false;
	}
	int cols = lab.getCols();
	int n = lab.getFilas() * cols;
	Node *p = L->getHead();
	if (p->getId() != f0 * cols + c0){
		return false;
	}
	int anterior = -1;
	int ultimo = -1;
	while (p != nullptr){
		int id = p->getId();
		if (id < 0 || id >= n || !lab.esLibre(id / cols, id % cols)){
			return false;
		}
		if (anterior >= 0){
			int df = id / cols - anterior / cols;
			int dc = id % cols - anterior % cols;
			if (df < 0){ df = -df; }
			if (dc < 0){ dc = -dc; }
			if (df + dc != 1){
				return false;
			}
		}
		anterior = id;
		ultimo = id;
		p = p->getNext();
	}
	return ultimo == f1 * cols + c1;
}

static bool sucesoresCeldaSon(Laberinto &lab, int f, int c, const int *esperado, int n){
	int suc[MAX_SUC_CELDA];
	for (int i = 0; i < MAX_SUC_CELDA; i++){
		suc[i] = -1;
	}
	int k = sucesoresCelda(lab, f * lab.getCols() + c, suc);
	if (k != n){
		return false;
	}
	for (int i = 0; i < n; i++){
		if (suc[i] != esperado[i]){
			return false;
		}
	}
	return true;
}

// Laberinto 1: la Figura 5.12 del libro, recortada a 6 x 6.
static const char *LAB1[] = {"..##.#",
                             ".#....",
                             "##....",
                             ".#..#.",
                             ".###..",
                             "..#..#"};

// Laberinto 2: hay un camino corto por la izquierda y uno largo por la vuelta.
static const char *LAB2[] = {"......",
                             ".#.##.",
                             ".#....",
                             ".####.",
                             "......",
                             ".####."};

// Laberinto 3: la meta esta encerrada.
static const char *LAB3[] = {"......",
                             ".####.",
                             ".#..#.",
                             ".#..#.",
                             ".####.",
                             "......"};

// Imprime los dos caminos, el de la pila y el de la cola, para comparar.
static void mostrarLaberinto(const char *titulo, Laberinto &lab, int f0, int c0, int f1, int c1){
	std::cout << std::endl << "  " << titulo << ": de (" << f0 << "," << c0 << ") a ("
	          << f1 << "," << c1 << ")" << std::endl;
	LList *pila = resolverLaberinto(lab, f0, c0, f1, c1);
	LList *cola = resolverLaberintoCola(lab, f0, c0, f1, c1);
	std::cout << "    con pila: ";
	if (pila != nullptr){ std::cout << largo(pila) - 1 << " pasos" << std::endl; }
	else{ std::cout << "no hay camino" << std::endl; }
	lab.print(pila);
	std::cout << "    con cola: ";
	if (cola != nullptr){ std::cout << largo(cola) - 1 << " pasos" << std::endl; }
	else{ std::cout << "no hay camino" << std::endl; }
	lab.print(cola);
	delete pila;
	delete cola;
}

static void testParte3(){
	parte("Parte 3 - El laberinto");
	Laberinto lab1(6, 6, LAB1);
	Laberinto lab2(6, 6, LAB2);
	Laberinto lab3(6, 6, LAB3);

	std::cout << "  3a - El modelo" << std::endl;
	check("idCelda(2, 3, 6) = 15", idCelda(2, 3, 6) == 15);
	check("filaDe(15, 6) = 2, colDe(15, 6) = 3", filaDe(15, 6) == 2 && colDe(15, 6) == 3);
	// Con 4 columnas cambia todo: atrapa a quien dejo el 6 fijo.
	check("idCelda(2, 3, 4) = 11 y de vuelta", idCelda(2, 3, 4) == 11 && filaDe(11, 4) == 2 && colDe(11, 4) == 3);

	const int de00[] = {1 * 6 + 0, 0 * 6 + 1};                  // S, E
	const int de22[] = {1 * 6 + 2, 2 * 6 + 3};                  // N, E
	const int de45[] = {3 * 6 + 5, 5 * 6 + 5, 4 * 6 + 4};       // N, S, O
	check("sucesores de (0,0) en lab 2", sucesoresCeldaSon(lab2, 0, 0, de00, 2));
	check("sucesores de (2,2) en lab 2", sucesoresCeldaSon(lab2, 2, 2, de22, 2));
	check("sucesores de (4,5) en lab 2", sucesoresCeldaSon(lab2, 4, 5, de45, 3));

	std::cout << std::endl << "  3b - resolverLaberinto (pila)" << std::endl;
	LList *L = resolverLaberinto(lab1, 1, 2, 5, 4);
	bool anda = caminoValido(lab1, L, 1, 2, 5, 4);
	check("lab 1: camino valido", anda);
	delete L;
	L = resolverLaberinto(lab2, 0, 0, 5, 0);
	check("lab 2: camino valido", caminoValido(lab2, L, 0, 0, 5, 0));
	delete L;
	check("lab 3: nullptr (meta encerrada)", anda && revisarYBorrar(resolverLaberinto(lab3, 0, 0, 2, 2), nullptr, 0));
	check("inicio sobre un muro: nullptr", anda && revisarYBorrar(resolverLaberinto(lab2, 1, 1, 5, 0), nullptr, 0));

	std::cout << std::endl << "  3c - resolverLaberintoCola" << std::endl;
	L = resolverLaberintoCola(lab1, 1, 2, 5, 4);
	anda = caminoValido(lab1, L, 1, 2, 5, 4) && largo(L) == 9;
	check("lab 1: valido y de 8 pasos", anda);
	delete L;
	L = resolverLaberintoCola(lab2, 0, 0, 5, 0);
	check("lab 2: valido y de 5 pasos", caminoValido(lab2, L, 0, 0, 5, 0) && largo(L) == 6);
	delete L;
	check("lab 3: nullptr (meta encerrada)", anda && revisarYBorrar(resolverLaberintoCola(lab3, 0, 0, 2, 2), nullptr, 0));

	mostrarLaberinto("Laberinto 1 (Figura 5.12 del libro)", lab1, 1, 2, 5, 4);
	mostrarLaberinto("Laberinto 2", lab2, 0, 0, 5, 0);
	mostrarLaberinto("Laberinto 3", lab3, 0, 0, 2, 2);
}

// ---------------------------------------------------------------------------

int main(int nargs, char **vargs){
	std::cout << "Laboratorio 5 - Busqueda de caminos con pilas y colas" << std::endl;

	testParte1();
	testParte2();
	testParte3();

	std::cout << std::endl << "Resumen: " << nOk << " de " << (nOk + nFalla)
	          << " correctas" << std::endl;
	if (nFalla > 0){
		std::cout << "Quedan " << nFalla << " por resolver." << std::endl;
	}
	return 0;
}
