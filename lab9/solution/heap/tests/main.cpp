#include "heap/heap.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// Cada prueba marca [ok] o [FALLA]; cuando falla, indica el motivo.
//
// Para revisar, este archivo recorre el heap por su cuenta, nivel por nivel,
// con los getters de HeapNode: revisa que el arbol sea completo, el padre de
// cada nodo, que ningun nodo tenga menor prioridad que su padre, que cada
// prioridad siga junto a su dato y cuantos nodos quedan en memoria. Nunca
// sigue un puntero a un nodo que ya se libero (HeapNode::estaVivo). Los
// arboles de la Parte 1 los arma el mismo, con setRoot.

static int nOk = 0;
static int nFalla = 0;

static void check(const std::string &nombre, bool ok, const std::string &motivo){
	std::cout << "  " << std::left << std::setw(52) << nombre
	          << (ok ? "[ok]" : "[FALLA]") << std::endl;
	if (!ok && !motivo.empty()){
		std::cout << "      " << motivo << std::endl;
	}
	if (ok){ nOk++; }
	else{ nFalla++; }
}

static void parte(const char *titulo){
	std::cout << std::endl << "==== " << titulo << std::endl;
}

static std::string num(int x){
	return std::to_string(x);
}

// Un nodo como texto (su prioridad), sin leer nunca un nodo ya liberado.
static std::string txt(HeapNode *node){
	if (node == nullptr){
		return "nullptr";
	}
	if (!HeapNode::estaVivo(node)){
		return "un nodo ya liberado";
	}
	return num(node->getPrioridad());
}

// Un dato del heap: (prioridad, dato).
typedef std::pair<int, int> Par;

static std::string par(const Par &p){
	return "el dato " + num(p.second) + " (prioridad " + num(p.first) + ")";
}

// ---------------------------------------------------------------------------
// Referencias propias del test
// ---------------------------------------------------------------------------

// Arma un arbol completo con estas prioridades en orden por niveles, sin
// usar push. El dato de cada nodo es 1000 + su prioridad. Deja en nodos los
// nodos en ese mismo orden.
static void armar(Heap &h, const std::vector<int> &prioridades, std::vector<HeapNode*> &nodos){
	nodos.clear();
	for (int i = 0; i < (int)prioridades.size(); i++){
		HeapNode *padre = (i == 0) ? nullptr : nodos[(i - 1) / 2];
		HeapNode *nodo = new HeapNode(1000 + prioridades[i], prioridades[i], padre);
		if (padre != nullptr){
			if (i % 2 == 1){
				padre->setLeft(nodo);
			}
			else{
				padre->setRight(nodo);
			}
		}
		nodos.push_back(nodo);
	}
	h.setRoot(nodos.empty() ? nullptr : nodos[0]);
}

static std::vector<Par> pares(const std::vector<int> &prioridades){
	std::vector<Par> v;
	for (int i = 0; i < (int)prioridades.size(); i++){
		v.push_back(Par(prioridades[i], 1000 + prioridades[i]));
	}
	return v;
}

// Un tope de nodos para no recorrer para siempre un arbol con un ciclo.
static const int TOPE = 100000;

// Las prioridades como texto: 1(2(4,5),3(6,-)), con - para un hijo que no
// existe. Una hoja es solo su prioridad.
static std::string forma(HeapNode *node, int &visitados){
	if (node == nullptr){
		return "-";
	}
	if (!HeapNode::estaVivo(node)){
		return "?";
	}
	visitados++;
	if (visitados > TOPE){
		return "...";
	}
	std::string s = num(node->getPrioridad());
	if (node->getLeft() != nullptr || node->getRight() != nullptr){
		s = s + "(" + forma(node->getLeft(), visitados) + "," + forma(node->getRight(), visitados) + ")";
	}
	return s;
}

static std::string forma(HeapNode *node){
	int visitados = 0;
	return forma(node, visitados);
}

// Revisa que h sea un heap bien armado con exactamente los datos esperados.
// base es cuantos nodos habia vivos antes de armarlo. Si conTamano es false,
// no revisa size(): los heaps armados con setRoot no tienen n al dia.
static bool revisar(Heap &h, std::vector<Par> esperados, int base, bool conTamano, std::string &motivo){
	std::vector<Par> encontrados;
	HeapNode *raiz = h.getRoot();
	if (raiz != nullptr){
		if (!HeapNode::estaVivo(raiz)){
			motivo = "la raiz del heap apunta a un nodo que ya se libero";
			return false;
		}
		if (raiz->getParent() != nullptr){
			motivo = "la raiz (" + txt(raiz) + ") tiene padre: " + txt(raiz->getParent());
			return false;
		}
		std::vector<HeapNode*> cola;
		cola.push_back(raiz);
		bool hueco = false;
		for (int i = 0; i < (int)cola.size(); i++){
			if ((int)cola.size() > TOPE){
				motivo = "el arbol tiene un ciclo: el recorrido no termina";
				return false;
			}
			HeapNode *p = cola[i];
			encontrados.push_back(Par(p->getPrioridad(), p->getDato()));
			HeapNode *hijos[2] = {p->getLeft(), p->getRight()};
			const char *lado[2] = {"izquierdo", "derecho"};
			for (int j = 0; j < 2; j++){
				HeapNode *c = hijos[j];
				if (c == nullptr){
					hueco = true;
					continue;
				}
				if (!HeapNode::estaVivo(c)){
					motivo = std::string("el hijo ") + lado[j] + " de " + txt(p) + " apunta a un nodo que ya se libero";
					return false;
				}
				if (hueco){
					motivo = "el arbol no es completo: hay un lugar vacio antes de " + txt(c)
					       + " (hijo " + lado[j] + " de " + txt(p) + ")";
					return false;
				}
				if (c->getParent() != p){
					motivo = "el padre de " + txt(c) + " es " + txt(c->getParent()) + " y deberia ser " + txt(p);
					return false;
				}
				if (c->getPrioridad() < p->getPrioridad()){
					motivo = txt(c) + " esta debajo de " + txt(p) + " y tiene menor prioridad";
					return false;
				}
				cola.push_back(c);
			}
		}
	}
	std::sort(esperados.begin(), esperados.end());
	std::sort(encontrados.begin(), encontrados.end());
	if (encontrados != esperados){
		int i = 0;
		while (i < (int)encontrados.size() && i < (int)esperados.size() && encontrados[i] == esperados[i]){
			i++;
		}
		if (i < (int)esperados.size() && (i >= (int)encontrados.size() || esperados[i] < encontrados[i])){
			motivo = "falta " + par(esperados[i]);
		}
		else{
			motivo = "en el heap esta " + par(encontrados[i]) + ", que no deberia estar";
		}
		return false;
	}
	if (conTamano && h.size() != (int)esperados.size()){
		motivo = "size() retorna " + num(h.size()) + " y el heap tiene " + num(esperados.size()) + " datos";
		return false;
	}
	if (conTamano && h.isEmpty() != esperados.empty()){
		motivo = "isEmpty() no coincide con la cantidad de datos";
		return false;
	}
	int enMemoria = HeapNode::nodosVivos() - base;
	int n = esperados.size();
	if (enMemoria > n){
		motivo = "hay " + num(enMemoria) + " nodos en memoria y el heap tiene " + num(n) + ": falta un delete";
		return false;
	}
	if (enMemoria < n){
		motivo = "hay " + num(enMemoria) + " nodos en memoria y el heap tiene " + num(n) + ": se libero un nodo de mas";
		return false;
	}
	return true;
}

static bool revisarForma(Heap &h, const std::string &esperada, std::string &motivo){
	std::string obtenida = forma(h.getRoot());
	if (obtenida != esperada){
		motivo = "se esperaba " + esperada + " y se obtuvo " + obtenida;
		return false;
	}
	return true;
}

// Un generador propio, para que los datos sean los mismos en todo computador.
static unsigned int semilla = 2026;

static int azar(int n){
	semilla = semilla * 1103515245u + 12345u;
	return (int)((semilla / 65536u) % (unsigned int)n);
}

// Saca un dato del heap con top y pop, revisando que sea el de menor
// prioridad. referencia tiene los datos que deberian estar en el heap, y el
// que sale se borra de ahi.
static bool sacar(Heap &h, std::vector<Par> &referencia, std::string &motivo){
	int menor = referencia[0].first;
	for (int i = 1; i < (int)referencia.size(); i++){
		menor = std::min(menor, referencia[i].first);
	}
	Par sale(h.topPrioridad(), h.top());
	if (sale.first != menor){
		motivo = "topPrioridad() retorna " + num(sale.first) + " y la menor prioridad del heap es " + num(menor);
		return false;
	}
	std::vector<Par>::iterator it = std::find(referencia.begin(), referencia.end(), sale);
	if (it == referencia.end()){
		motivo = "top() retorna " + num(sale.second) + " con prioridad " + num(sale.first)
		       + ", y ese dato no esta en el heap con esa prioridad";
		return false;
	}
	referencia.erase(it);
	h.pop();
	return true;
}

// ===========================================================================
// Parte 1 - Reparar
// ===========================================================================

static void probarReparar(const std::string &nombre, const std::vector<int> &prioridades,
                          bool desdeAbajo, const std::string &esperada){
	int base = HeapNode::nodosVivos();
	Heap h;
	std::vector<HeapNode*> nodos;
	armar(h, prioridades, nodos);
	if (desdeAbajo){
		h.repareFromBottom(nodos[nodos.size() - 1]);
	}
	else{
		h.repareFromTop(nodos[0]);
	}
	std::string motivo;
	bool ok = revisar(h, pares(prioridades), base, false, motivo) && revisarForma(h, esperada, motivo);
	check(nombre, ok, motivo);
	if (ok){
		h.imprimir();
	}
}

static void testReparar(){
	parte("Parte 1 - Reparar");
	probarReparar("repareFromBottom: el 1 sube hasta la raiz", {10, 20, 30, 40, 50, 60, 1}, true,
	              "1(20(40,50),10(60,30))");
	probarReparar("repareFromBottom: el 15 se detiene bajo el 10", {10, 20, 30, 40, 50, 60, 15}, true,
	              "10(20(40,50),15(60,30))");
	probarReparar("repareFromTop: el 70 baja por el hijo menor", {70, 20, 10, 40, 50, 60, 30}, false,
	              "10(20(40,50),30(60,70))");
	probarReparar("repareFromTop: un nodo con un solo hijo", {50, 10, 30, 20}, false,
	              "10(20(50,-),30)");
}

// ===========================================================================
// Parte 2 - push y top
// ===========================================================================

static void probarPush(const std::string &nombre, const std::vector<int> &prioridades,
                       const std::string &esperada){
	int base = HeapNode::nodosVivos();
	Heap h;
	for (int i = 0; i < (int)prioridades.size(); i++){
		h.push(1000 + prioridades[i], prioridades[i]);
	}
	std::string motivo;
	bool ok = revisar(h, pares(prioridades), base, true, motivo) && revisarForma(h, esperada, motivo);
	check(nombre, ok, motivo);
	if (ok){
		h.imprimir();
	}
}

static void testTopAlAzar(){
	const int N = 1000;
	int base = HeapNode::nodosVivos();
	Heap h;
	std::vector<Par> referencia;
	int menor = 0;
	bool ok = true;
	std::string motivo;
	for (int i = 0; ok && i < N; i++){
		Par nuevo(azar(100), i);
		h.push(nuevo.second, nuevo.first);
		referencia.push_back(nuevo);
		menor = (i == 0) ? nuevo.first : std::min(menor, nuevo.first);
		if (h.size() != i + 1){
			ok = false;
			motivo = "despues de " + num(i + 1) + " push, size() retorna " + num(h.size());
		}
		else if (h.topPrioridad() != menor){
			ok = false;
			motivo = "despues de " + num(i + 1) + " push, topPrioridad() retorna " + num(h.topPrioridad())
			       + " y la menor es " + num(menor);
		}
		else if (std::find(referencia.begin(), referencia.end(), Par(h.topPrioridad(), h.top())) == referencia.end()){
			ok = false;
			motivo = "top() retorna " + num(h.top()) + ", que no es un dato con prioridad " + num(menor);
		}
	}
	if (ok){
		ok = revisar(h, referencia, base, true, motivo);
	}
	check("top: siempre la menor prioridad (1000 push al azar)", ok, motivo);
}

static void testPush(){
	parte("Parte 2 - push y top");
	probarPush("push llena el arbol por niveles: 1 a 7", {1, 2, 3, 4, 5, 6, 7}, "1(2(4,5),3(6,7))");
	probarPush("push repara hacia arriba: 7 a 1", {7, 6, 5, 4, 3, 2, 1}, "1(4(7,5),2(6,3))");
	testTopAlAzar();
}

// ===========================================================================
// Parte 3 - pop
// ===========================================================================

static void testPopUnico(){
	int base = HeapNode::nodosVivos();
	Heap h;
	h.push(5, 50);
	std::string motivo;
	bool ok = revisar(h, {Par(50, 5)}, base, true, motivo);
	if (!ok){
		motivo = "push no agrega el dato (revisa la Parte 2): " + motivo;
	}
	if (ok){
		h.pop();
		ok = revisar(h, {}, base, true, motivo);
	}
	if (ok && h.getRoot() != nullptr){
		ok = false;
		motivo = "el heap quedo vacio y la raiz no es nullptr";
	}
	check("pop de un heap con un solo dato", ok, motivo);
}

static void testPopFigura(){
	int base = HeapNode::nodosVivos();
	Heap h;
	std::vector<int> prioridades = {1, 2, 3, 4, 5, 6, 7};
	for (int i = 0; i < 7; i++){
		h.push(1000 + prioridades[i], prioridades[i]);
	}
	std::string motivo;
	bool ok = revisar(h, pares(prioridades), base, true, motivo);
	if (!ok){
		motivo = "push no arma el heap (revisa la Parte 2): " + motivo;
	}
	if (ok){
		h.pop();
		ok = revisar(h, pares({2, 3, 4, 5, 6, 7}), base, true, motivo)
		  && revisarForma(h, "2(4(7,5),3(6,-))", motivo);
	}
	check("pop: el ultimo sube a la raiz y baja", ok, motivo);
	if (ok){
		h.imprimir();
	}
}

// push y pop intercalados: despues de cada pop, el proximo push tiene que
// caer en el lugar que quedo libre. La lista padres tiene que estar al dia.
static void testAlternados(){
	int base = HeapNode::nodosVivos();
	Heap h;
	std::vector<Par> referencia;
	// 0 = pop; cualquier otro numero = push de esa prioridad.
	const int OPS[] = {1, 2, 3, 4, 5, 6, 7, 0, 8, 0, 0, 0, 9, 10, 0, 11, 12, 13, 0, 0, 14};
	const int N_OPS = sizeof(OPS) / sizeof(OPS[0]);
	bool ok = true;
	std::string motivo;
	for (int i = 0; ok && i < N_OPS; i++){
		std::string op;
		if (OPS[i] == 0){
			op = "pop";
			ok = sacar(h, referencia, motivo);
		}
		else{
			op = "push de " + num(OPS[i]);
			h.push(1000 + OPS[i], OPS[i]);
			referencia.push_back(Par(OPS[i], 1000 + OPS[i]));
		}
		if (ok){
			ok = revisar(h, referencia, base, true, motivo);
		}
		if (!ok){
			motivo = "en la operacion " + num(i + 1) + " (" + op + "): " + motivo;
		}
	}
	check("pop y push alternados: el arbol sigue completo", ok, motivo);
}

// HeapSort con el heap (libro, 6.8): todo entra con push y sale con pop.
static void testOrdenar(){
	const int N = 1000;
	int base = HeapNode::nodosVivos();
	Heap h;
	std::vector<Par> referencia;
	for (int i = 0; i < N; i++){
		Par nuevo(azar(100), i);
		h.push(nuevo.second, nuevo.first);
		referencia.push_back(nuevo);
	}
	std::string motivo;
	bool ok = revisar(h, referencia, base, true, motivo);
	if (!ok){
		motivo = "despues de los " + num(N) + " push: " + motivo;
	}
	for (int i = 0; ok && i < N; i++){
		ok = sacar(h, referencia, motivo);
		if (!ok){
			motivo = "en el pop " + num(i + 1) + ": " + motivo;
		}
	}
	if (ok){
		ok = revisar(h, {}, base, true, motivo);
	}
	check("ordenar con el heap: 1000 push y 1000 pop", ok, motivo);
}

static void testMezcla(){
	const int N_OPS = 5000;
	int base = HeapNode::nodosVivos();
	Heap h;
	std::vector<Par> referencia;
	bool ok = true;
	std::string motivo;
	for (int i = 0; ok && i < N_OPS; i++){
		if (referencia.empty() || azar(10) < 6){
			Par nuevo(azar(100), i);
			h.push(nuevo.second, nuevo.first);
			referencia.push_back(nuevo);
		}
		else{
			ok = sacar(h, referencia, motivo);
		}
		if (ok && h.size() != (int)referencia.size()){
			ok = false;
			motivo = "size() retorna " + num(h.size()) + " y el heap tiene " + num(referencia.size()) + " datos";
		}
		if (ok && (i % 50 == 0 || i == N_OPS - 1)){
			ok = revisar(h, referencia, base, true, motivo);
		}
		if (!ok){
			motivo = "en la operacion " + num(i + 1) + ": " + motivo;
		}
	}
	check("5000 push y pop al azar", ok, motivo);
}

static void testPopVacio(){
	int base = HeapNode::nodosVivos();
	Heap h;
	h.pop();
	std::string motivo;
	bool ok = revisar(h, {}, base, true, motivo);
	if (!ok){
		motivo = "pop de un heap vacio cambia el heap: " + motivo;
	}
	if (ok){
		h.push(7, 70);
		ok = revisar(h, {Par(70, 7)}, base, true, motivo);
		if (!ok){
			motivo = "despues de un pop vacio, push(7, 70): " + motivo;
		}
	}
	check("pop de un heap vacio no hace nada", ok, motivo);
}

static void testPop(){
	parte("Parte 3 - pop");
	testPopUnico();
	testPopFigura();
	testAlternados();
	testOrdenar();
	testMezcla();
	testPopVacio();
}

// ---------------------------------------------------------------------------

int main(int nargs, char **vargs){
	std::cout << "Laboratorio 9 - Heap" << std::endl;

	testReparar();
	testPush();
	testPop();

	std::cout << std::endl << "Resumen: " << nOk << " de " << (nOk + nFalla)
	          << " correctas" << std::endl;
	return 0;
}
