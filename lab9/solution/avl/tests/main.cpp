#include "avl/avl.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// Cada prueba marca [ok] o [FALLA]; cuando falla, indica el motivo.
//
// Para revisar, este archivo recorre los arboles por su cuenta, con los
// getters de AVLNode: revisa el orden de las llaves, el padre de cada nodo,
// la altura guardada en cada nodo y, cuando corresponde, que el arbol este
// balanceado. Los arboles de las Partes 1 y 2 los arma el mismo, con setRoot,
// asi que esas partes no dependen de insert.

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

static std::string txt(AVLNode *node){
	return node == nullptr ? "nullptr" : num(node->getData());
}

// ---------------------------------------------------------------------------
// Referencias propias del test
// ---------------------------------------------------------------------------

static int alturaReal(AVLNode *node){
	if (node == nullptr){
		return -1;
	}
	return 1 + std::max(alturaReal(node->getLeft()), alturaReal(node->getRight()));
}

static void fijarAlturas(AVLNode *node){
	if (node != nullptr){
		fijarAlturas(node->getLeft());
		fijarAlturas(node->getRight());
		node->setAltura(alturaReal(node));
	}
}

// Arma un ABB con las llaves en ese orden, sin balancear y sin usar insert,
// con las alturas correctas.
static void construir(AVL &avl, const std::vector<int> &llaves){
	AVLNode *raiz = nullptr;
	for (int i = 0; i < (int)llaves.size(); i++){
		int k = llaves[i];
		if (raiz == nullptr){
			raiz = new AVLNode(k);
		}
		else{
			AVLNode *p = raiz;
			bool listo = false;
			while (!listo){
				if (k < p->getData()){
					if (p->getLeft() == nullptr){
						p->setLeft(new AVLNode(k, p));
						listo = true;
					}
					else{
						p = p->getLeft();
					}
				}
				else{
					if (p->getRight() == nullptr){
						p->setRight(new AVLNode(k, p));
						listo = true;
					}
					else{
						p = p->getRight();
					}
				}
			}
		}
	}
	fijarAlturas(raiz);
	avl.setRoot(raiz);
}

static AVLNode *buscar(AVLNode *p, int k){
	while (p != nullptr && p->getData() != k){
		p = (k < p->getData()) ? p->getLeft() : p->getRight();
	}
	return p;
}

// Un tope de nodos para no recorrer para siempre un arbol con un ciclo.
static const int TOPE = 100000;

// La forma del arbol como texto: 20(10,30), con - para un hijo que no
// existe. Una hoja es solo su llave.
static std::string forma(AVLNode *node, int &visitados){
	if (node == nullptr){
		return "-";
	}
	visitados++;
	if (visitados > TOPE){
		return "...";
	}
	std::string s = num(node->getData());
	if (node->getLeft() != nullptr || node->getRight() != nullptr){
		s = s + "(" + forma(node->getLeft(), visitados) + "," + forma(node->getRight(), visitados) + ")";
	}
	return s;
}

static std::string forma(AVLNode *node){
	int visitados = 0;
	return forma(node, visitados);
}

// Revisa el subarbol de node: el padre, el orden respecto de los ancestros,
// la altura guardada y, si balanceado es true, el factor de balance. Retorna
// la altura real del subarbol, o -2 si encontro un error.
static int revisarNodo(AVLNode *node, AVLNode *padre, AVLNode *menorQue, AVLNode *mayorQue,
                       bool balanceado, std::vector<int> &llaves, int &visitados, std::string &motivo){
	if (node == nullptr){
		return -1;
	}
	visitados++;
	if (visitados > TOPE){
		motivo = "el arbol tiene un ciclo: el recorrido no termina";
		return -2;
	}
	if (node->getParent() != padre){
		motivo = "el padre de " + txt(node) + " es " + txt(node->getParent()) + " y deberia ser " + txt(padre);
		return -2;
	}
	if (menorQue != nullptr && !(node->getData() < menorQue->getData())){
		motivo = txt(node) + " esta en el subarbol izquierdo de " + txt(menorQue) + " y no es menor";
		return -2;
	}
	if (mayorQue != nullptr && !(node->getData() > mayorQue->getData())){
		motivo = txt(node) + " esta en el subarbol derecho de " + txt(mayorQue) + " y no es mayor";
		return -2;
	}
	int hl = revisarNodo(node->getLeft(), node, node, mayorQue, balanceado, llaves, visitados, motivo);
	if (hl == -2){
		return -2;
	}
	llaves.push_back(node->getData());
	int hr = revisarNodo(node->getRight(), node, menorQue, node, balanceado, llaves, visitados, motivo);
	if (hr == -2){
		return -2;
	}
	int h = 1 + std::max(hl, hr);
	if (node->getAltura() != h){
		motivo = "la altura guardada en " + txt(node) + " es " + num(node->getAltura()) + " y deberia ser " + num(h);
		return -2;
	}
	if (balanceado && (hl - hr > 1 || hl - hr < -1)){
		motivo = "el nodo " + txt(node) + " quedo desbalanceado: factor de balance " + num(hl - hr);
		return -2;
	}
	return h;
}

// Revisa que avl sea un ABB bien armado, con exactamente las llaves
// esperadas y las alturas al dia, y si balanceado es true, que sea un AVL.
static bool revisar(AVL &avl, std::vector<int> esperadas, bool balanceado, std::string &motivo){
	std::sort(esperadas.begin(), esperadas.end());
	std::vector<int> llaves;
	int visitados = 0;
	if (revisarNodo(avl.getRoot(), nullptr, nullptr, nullptr, balanceado, llaves, visitados, motivo) == -2){
		return false;
	}
	if (llaves != esperadas){
		motivo = "el arbol no tiene las llaves que deberia: se esperaban " + num(esperadas.size())
		       + " y tiene " + num(llaves.size());
		return false;
	}
	return true;
}

// Arma el arbol con llaves, aplica la operacion sobre el nodo de llave
// cual y compara la forma que queda con la esperada.
enum Operacion { LEFT, RIGHT, BALANCE };

static void probar(const std::string &nombre, const std::vector<int> &llaves, int cual,
                   Operacion op, const std::string &esperada, bool dibujar){
	AVL avl;
	construir(avl, llaves);
	AVLNode *node = buscar(avl.getRoot(), cual);
	if (op == LEFT){
		avl.leftRotation(node);
	}
	else if (op == RIGHT){
		avl.rightRotation(node);
	}
	else{
		avl.balance(node);
	}
	std::string motivo;
	bool ok = revisar(avl, llaves, false, motivo);
	if (ok){
		std::string obtenida = forma(avl.getRoot());
		if (obtenida != esperada){
			ok = false;
			motivo = "se esperaba la forma " + esperada + " y se obtuvo " + obtenida;
		}
	}
	check(nombre, ok, motivo);
	if (ok && dibujar){
		avl.imprimir();
	}
}

// ===========================================================================
// Parte 1 - Rotaciones simples
// ===========================================================================

static void testRotaciones(){
	parte("Parte 1 - Rotaciones simples");
	probar("leftRotation en la raiz: 10 20 30", {10, 20, 30}, 10, LEFT, "20(10,30)", false);
	probar("rightRotation en la raiz: 30 20 10", {30, 20, 10}, 30, RIGHT, "20(10,30)", false);
	// Figura 6.18: el hijo izquierdo de X (30) pasa a ser el hijo derecho de P.
	probar("leftRotation con subarboles (Figura 6.18)", {20, 10, 40, 30, 50}, 20, LEFT,
	       "40(20(10,30),50)", true);
	// Figura 6.19: el hijo derecho de X (30) pasa a ser el hijo izquierdo de P.
	probar("rightRotation con subarboles (Figura 6.19)", {40, 20, 50, 10, 30}, 40, RIGHT,
	       "20(10,40(30,50))", true);
	probar("leftRotation de un hijo izquierdo: 20", {50, 20, 60, 10, 30, 25, 40}, 20, LEFT,
	       "50(30(20(10,25),40),60)", false);
	probar("rightRotation de un hijo derecho: 70", {50, 40, 70, 60, 80, 55, 65}, 70, RIGHT,
	       "50(40,60(55,70(65,80)))", false);
	probar("leftRotation de un hijo derecho: 60", {50, 40, 60, 55, 70, 65, 80}, 60, LEFT,
	       "50(40,70(60(55,65),80))", false);
	probar("rightRotation de un hijo izquierdo: 30", {50, 30, 60, 20, 40, 10, 25}, 30, RIGHT,
	       "50(20(10,30(25,40)),60)", false);
}

// ===========================================================================
// Parte 2 - Balancear
// ===========================================================================

static void testBalance(){
	parte("Parte 2 - Balancear");
	probar("rotacion Right: 30 20 10", {30, 20, 10}, 30, BALANCE, "20(10,30)", false);
	probar("rotacion Left: 10 20 30", {10, 20, 30}, 10, BALANCE, "20(10,30)", false);
	probar("rotacion Left-Right: 30 10 20", {30, 10, 20}, 30, BALANCE, "20(10,30)", false);
	probar("rotacion Right-Left: 10 30 20", {10, 30, 20}, 10, BALANCE, "20(10,30)", false);
	probar("Left-Right con subarboles (Figura 6.20)", {50, 20, 60, 10, 30, 25, 35}, 50, BALANCE,
	       "30(20(10,25),50(35,60))", true);
	probar("Right-Left con subarboles (Figura 6.21)", {20, 10, 50, 40, 60, 35, 45}, 20, BALANCE,
	       "40(20(10,35),50(45,60))", true);
}

// ===========================================================================
// Parte 3 - Insertar en un AVL
// ===========================================================================

// Un generador propio, para que las llaves sean las mismas en todo computador.
static unsigned int semilla = 2026;

static int azar(int n){
	semilla = semilla * 1103515245u + 12345u;
	return (int)((semilla / 65536u) % (unsigned int)n);
}

static void insertarTodas(const std::string &nombre, const std::vector<int> &llaves,
                          const std::string &esperada, const std::string &comparacion){
	AVL avl;
	bool ok = true;
	std::string motivo;
	for (int i = 0; ok && i < (int)llaves.size(); i++){
		if (!avl.insert(llaves[i])){
			ok = false;
			motivo = "insert(" + num(llaves[i]) + ") retorna false con una llave nueva";
		}
	}
	if (ok){
		ok = revisar(avl, llaves, true, motivo);
	}
	if (ok && !esperada.empty()){
		std::string obtenida = forma(avl.getRoot());
		if (obtenida != esperada){
			ok = false;
			motivo = "se esperaba la forma " + esperada + " y se obtuvo " + obtenida;
		}
	}
	check(nombre, ok, motivo);
	if (ok && esperada.empty()){
		std::cout << "      altura " << alturaReal(avl.getRoot()) << comparacion << std::endl;
	}
	if (ok && !esperada.empty()){
		avl.imprimir();
	}
}

static void testInsertar(){
	parte("Parte 3 - Insertar en un AVL");
	insertarTodas("16 32 45 8 10 15 (el ejemplo de la clase)", {16, 32, 45, 8, 10, 15},
	              "16(10(8,15),32(-,45))", "");

	std::vector<int> ordenadas;
	for (int i = 1; i <= 1000; i++){
		ordenadas.push_back(i);
	}
	insertarTodas("las llaves 1 a 1000, en orden", ordenadas, "",
	              " (un ABB sin balancear quedaria de altura 999)");

	std::vector<int> todas;
	for (int i = 0; i < 3000; i++){
		todas.push_back(i);
	}
	for (int i = (int)todas.size() - 1; i > 0; i--){
		std::swap(todas[i], todas[azar(i + 1)]);
	}
	std::vector<int> alAzar(todas.begin(), todas.begin() + 1000);
	insertarTodas("1000 llaves al azar", alAzar, "",
	              " (el ABB del laboratorio 8, sin balancear, quedaba de altura 19)");
}

// ---------------------------------------------------------------------------

int main(int nargs, char **vargs){
	std::cout << "Laboratorio 9 - Arbol AVL" << std::endl;

	testRotaciones();
	testBalance();
	testInsertar();

	std::cout << std::endl << "Resumen: " << nOk << " de " << (nOk + nFalla)
	          << " correctas" << std::endl;
	return 0;
}
