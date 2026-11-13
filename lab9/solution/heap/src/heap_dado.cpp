#include "heap/heap.hpp"
#include <iostream>
#include <string>
#include <vector>

// ===========================================================================
// Lo que viene dado de la clase Heap. Este archivo no se modifica.
// ===========================================================================

Heap::Heap(): root(nullptr), n(0){
}

// Un puntero a un nodo que ya se libero no se sigue: asi un error en pop no
// hace caer el programa cuando el heap se destruye.
void Heap::destruir(HeapNode* node){
	if (node != nullptr && HeapNode::estaVivo(node)){
		destruir(node->getLeft());
		destruir(node->getRight());
		delete node;
	}
}

Heap::~Heap(){
	destruir(root);
}

HeapNode* Heap::getRoot(){
	return root;
}

void Heap::setRoot(HeapNode* node){
	root = node;
}

int Heap::size(){
	return n;
}

bool Heap::isEmpty(){
	return n == 0;
}

// ---------------------------------------------------------------------------
// imprimir
// ---------------------------------------------------------------------------

static int alturaDe(HeapNode* node){
	if (node == nullptr || !HeapNode::estaVivo(node)){
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

static void ubicar(HeapNode* node, int nivel, int pos, int ancho, int centroPadre,
                   const std::string &linea, std::vector<std::string> &filas){
	if (node == nullptr || !HeapNode::estaVivo(node)){
		return;
	}
	int centro = (2 * pos + 1) * ancho / (1 << (nivel + 1));
	if (nivel > 0){
		escribir(filas[2 * nivel - 1], (centro + centroPadre) / 2, linea);
	}
	std::string s = std::to_string(node->getPrioridad());
	escribir(filas[2 * nivel], centro - (int)s.size() / 2, s);
	ubicar(node->getLeft(), nivel + 1, 2 * pos, ancho, centro, "/", filas);
	ubicar(node->getRight(), nivel + 1, 2 * pos + 1, ancho, centro, "\\", filas);
}

// Dibuja las prioridades de arriba hacia abajo. Un heap de mas de 5 niveles
// (mas de 31 datos) no cabe en la pantalla y se describe en una linea.
void Heap::imprimir(){
	if (root == nullptr){
		std::cout << "      (heap vacio)" << std::endl;
		return;
	}
	int h = alturaDe(root);
	if (h > 4){
		std::cout << "      (heap de " << n << " datos y " << h + 1 << " niveles)" << std::endl;
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
