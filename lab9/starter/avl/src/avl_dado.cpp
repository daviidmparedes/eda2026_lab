#include "avl/avl.hpp"
#include <iostream>
#include <string>
#include <vector>

// ===========================================================================
// Lo que viene dado de la clase AVL. Este archivo no se modifica.
// ===========================================================================

AVL::AVL(): root(nullptr){
}

void AVL::destruir(AVLNode* node){
	if (node != nullptr){
		destruir(node->getLeft());
		destruir(node->getRight());
		delete node;
	}
}

AVL::~AVL(){
	destruir(root);
}

AVLNode* AVL::getRoot(){
	return root;
}

void AVL::setRoot(AVLNode* node){
	root = node;
}

// ---------------------------------------------------------------------------
// Alturas
// ---------------------------------------------------------------------------

int AVL::altura(AVLNode* node){
	if (node == nullptr){
		return -1;
	}
	return node->getAltura();
}

void AVL::actualizarAltura(AVLNode* node){
	int hl = altura(node->getLeft());
	int hr = altura(node->getRight());
	node->setAltura(1 + (hl > hr ? hl : hr));
}

int AVL::factorBalance(AVLNode* node){
	return altura(node->getLeft()) - altura(node->getRight());
}

// ---------------------------------------------------------------------------
// Insertar
// ---------------------------------------------------------------------------

// La insercion del laboratorio 8. Lo nuevo esta despues de la llamada
// recursiva: de vuelta hacia la raiz, cada nodo del camino recalcula su
// altura y, si quedo desbalanceado, se balancea. Despues de balance, node
// ya no esta en el mismo lugar, pero su padre recalcula su altura con el hijo
// que quedo ahi.
bool AVL::insertRec(int key, AVLNode* node){
	bool insertado = false;
	if (key < node->getData()){
		if (node->getLeft() == nullptr){
			node->setLeft(new AVLNode(key, node));
			insertado = true;
		}
		else{
			insertado = insertRec(key, node->getLeft());
		}
	}
	else if (key > node->getData()){
		if (node->getRight() == nullptr){
			node->setRight(new AVLNode(key, node));
			insertado = true;
		}
		else{
			insertado = insertRec(key, node->getRight());
		}
	}
	if (insertado){
		actualizarAltura(node);
		int fb = factorBalance(node);
		if (fb > 1 || fb < -1){
			balance(node);
		}
	}
	return insertado;
}

bool AVL::insert(int key){
	bool insertado = false;
	if (root == nullptr){
		root = new AVLNode(key);
		insertado = true;
	}
	else{
		insertado = insertRec(key, root);
	}
	return insertado;
}

// ---------------------------------------------------------------------------
// imprimir
// ---------------------------------------------------------------------------

static int alturaReal(AVLNode* node){
	if (node == nullptr){
		return -1;
	}
	int hl = alturaReal(node->getLeft());
	int hr = alturaReal(node->getRight());
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
static void ubicar(AVLNode* node, int nivel, int pos, int ancho, int centroPadre,
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

static void imprimirAcostado(AVLNode* node, int nivel){
	if (node != nullptr){
		imprimirAcostado(node->getRight(), nivel + 1);
		std::cout << "      " << std::string(4 * nivel, ' ') << node->getData() << std::endl;
		imprimirAcostado(node->getLeft(), nivel + 1);
	}
}

// Un arbol de hasta 5 niveles se dibuja de arriba hacia abajo, como en el
// libro. Uno mas alto no cabe en la pantalla, y se dibuja acostado: la raiz a
// la izquierda y el subarbol derecho arriba.
void AVL::imprimir(){
	if (root == nullptr){
		std::cout << "      (arbol vacio)" << std::endl;
		return;
	}
	int h = alturaReal(root);
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
