#include "abb/abb.hpp"
#include <iostream>
#include <string>
#include <vector>

// ===========================================================================
// Lo que viene dado de la clase ABB. Este archivo no se modifica.
// ===========================================================================

ABB::ABB(): root(nullptr){
}

// En pos-orden: primero los dos subarboles y despues el nodo, porque una vez
// liberado el nodo ya no se pueden leer sus hijos. Un puntero a un nodo que
// ya se libero no se sigue: asi un error en remove no hace caer el programa
// cuando el arbol se destruye.
void ABB::destruir(ABBNode* node){
	if (node != nullptr && ABBNode::estaVivo(node)){
		destruir(node->getLeft());
		destruir(node->getRight());
		delete node;
	}
}

ABB::~ABB(){
	destruir(root);
}

ABBNode* ABB::getRoot(){
	return root;
}

void ABB::setRoot(ABBNode* node){
	root = node;
}

// ---------------------------------------------------------------------------
// imprimir
// ---------------------------------------------------------------------------

static int alturaDe(ABBNode* node){
	if (node == nullptr){
		return -1;
	}
	int hl = alturaDe(node->getLeft());
	int hr = alturaDe(node->getRight());
	return 1 + (hl > hr ? hl : hr);
}

static void escribir(std::string &fila, int col, const std::string &s){
	for (int i = 0; i < (int)s.size(); i++){
		if (col + i >= 0 && col + i < (int)fila.size()){
			fila[col + i] = s[i];
		}
	}
}

// Ubica cada nodo en su fila: la fila 2*nivel tiene las llaves y la fila de
// arriba, las lineas que lo unen con su padre.
static void ubicar(ABBNode* node, int nivel, int pos, int ancho, int centroPadre,
                   const std::string &linea, std::vector<std::string> &filas){
	if (node == nullptr){
		return;
	}
	int centro = (2 * pos + 1) * ancho / (1 << (nivel + 1));
	if (nivel > 0){
		escribir(filas[2 * nivel - 1], (centro + centroPadre) / 2, linea);
	}
	std::string s = std::to_string(node->getData());
	escribir(filas[2 * nivel], centro - (int)s.size() / 2, s);
	ubicar(node->getLeft(), nivel + 1, 2 * pos, ancho, centro, "/", filas);
	ubicar(node->getRight(), nivel + 1, 2 * pos + 1, ancho, centro, "\\", filas);
}

static void imprimirAcostado(ABBNode* node, int nivel){
	if (node != nullptr){
		imprimirAcostado(node->getRight(), nivel + 1);
		std::cout << "      " << std::string(4 * nivel, ' ') << node->getData() << std::endl;
		imprimirAcostado(node->getLeft(), nivel + 1);
	}
}

// Un arbol de hasta 5 niveles se dibuja de arriba hacia abajo, como en el
// libro. Uno mas alto no cabe en la pantalla, y se dibuja acostado: la raiz a
// la izquierda y el subarbol derecho arriba.
void ABB::imprimir(){
	if (root == nullptr){
		std::cout << "      (arbol vacio)" << std::endl;
		return;
	}
	int h = alturaDe(root);
	if (h > 4){
		imprimirAcostado(root, 0);
		return;
	}
	int ancho = (1 << h) * 4;
	std::vector<std::string> filas(2 * h + 1, std::string(ancho + 4, ' '));
	ubicar(root, 0, 0, ancho, 0, "", filas);
	for (int i = 0; i < (int)filas.size(); i++){
		std::string f = filas[i];
		while (!f.empty() && f[f.size() - 1] == ' '){
			f.erase(f.size() - 1);
		}
		std::cout << "      " << f << std::endl;
	}
}
