#include "heap/heap.hpp"

// ===========================================================================
// SOLUCION DE REFERENCIA
// ===========================================================================

// ---------------------------------------------------------------------------
// Parte 1 - Reparar
// ---------------------------------------------------------------------------

// Libro, 6.6.0.2. Solo se intercambian datos: cada nodo sigue en su lugar del
// arbol (y en la lista padres, si esta), y lo que sube es su contenido.
void Heap::repareFromBottom(HeapNode* node){
	HeapNode* p = node;
	while (p->getParent() != nullptr && p->getPrioridad() < p->getParent()->getPrioridad()){
		p->intercambiar(p->getParent());
		p = p->getParent();
	}
}

// Libro, 6.6.2. Se baja por el hijo de menor prioridad: el que sube queda
// arriba de los dos, asi que tiene que ser el menor.
void Heap::repareFromTop(HeapNode* node){
	HeapNode* p = node;
	bool listo = false;
	while (!listo){
		HeapNode* menor = p;
		if (p->getLeft() != nullptr && p->getLeft()->getPrioridad() < menor->getPrioridad()){
			menor = p->getLeft();
		}
		if (p->getRight() != nullptr && p->getRight()->getPrioridad() < menor->getPrioridad()){
			menor = p->getRight();
		}
		if (menor == p){
			listo = true;
		}
		else{
			p->intercambiar(menor);
			p = menor;
		}
	}
}

// ---------------------------------------------------------------------------
// Parte 2 - push y top
// ---------------------------------------------------------------------------

// Libro, 6.6.0.1. El primero de padres recibe al nodo nuevo: a la izquierda
// si esta libre, y si no a la derecha, y entonces ya tiene sus dos hijos y
// sale de la lista. El nodo nuevo entra al final: mas adelante sera padre.
void Heap::push(int dato, int prioridad){
	HeapNode* nuevo = new HeapNode(dato, prioridad);
	if (root == nullptr){
		root = nuevo;
	}
	else{
		HeapNode* p = padres.front();
		nuevo->setParent(p);
		if (p->getLeft() == nullptr){
			p->setLeft(nuevo);
		}
		else{
			p->setRight(nuevo);
			padres.popFront();
		}
	}
	padres.pushBack(nuevo);
	n = n + 1;
	repareFromBottom(nuevo);
}

int Heap::top(){
	return root->getDato();
}

int Heap::topPrioridad(){
	return root->getPrioridad();
}

// ---------------------------------------------------------------------------
// Parte 3 - pop
// ---------------------------------------------------------------------------

// Libro, 6.6.2. El ultimo nodo insertado es el ultimo de padres (una hoja
// siempre tiene lugar para hijos). Su dato pasa a la raiz, y el nodo se
// desengancha y se libera. Su padre recupera un lugar libre:
//   - si el ultimo era hijo izquierdo, el padre ya era el primero de padres
//     (le faltaba el derecho) y ahi sigue;
//   - si era hijo derecho, el padre habia salido de la lista y tiene que
//     volver, al principio: es el que esta mas arriba y mas a la izquierda.
void Heap::pop(){
	if (root == nullptr){
		return;
	}
	HeapNode* ultimo = padres.back();
	padres.popBack();
	if (ultimo == root){
		root = nullptr;
	}
	else{
		root->intercambiar(ultimo);
		HeapNode* p = ultimo->getParent();
		if (ultimo->isLeft()){
			p->setLeft(nullptr);
		}
		else{
			p->setRight(nullptr);
			padres.pushFront(p);
		}
	}
	delete ultimo;
	n = n - 1;
	if (root != nullptr){
		repareFromTop(root);
	}
}
