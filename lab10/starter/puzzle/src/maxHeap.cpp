#include "puzzle/maxHeap.hpp"

MaxHeap::MaxHeap(): root(nullptr){
}

bool MaxHeap::isEmpty(){
	return root == nullptr;
}

// Sube mientras tenga mayor prioridad que su padre.
void MaxHeap::repareFromBottom(HeapNode* node){
	HeapNode* p = node;
	while (p->getParent() != nullptr && p->getPrioridad() > p->getParent()->getPrioridad()){
		p->intercambiar(p->getParent());
		p = p->getParent();
	}
}

// Baja por el hijo de mayor prioridad, mientras alguno sea mayor que el nodo.
void MaxHeap::repareFromTop(HeapNode* node){
	HeapNode* p = node;
	bool listo = false;
	while (!listo){
		HeapNode* mayor = p;
		if (p->getLeft() != nullptr && p->getLeft()->getPrioridad() > mayor->getPrioridad()){
			mayor = p->getLeft();
		}
		if (p->getRight() != nullptr && p->getRight()->getPrioridad() > mayor->getPrioridad()){
			mayor = p->getRight();
		}
		if (mayor == p){
			listo = true;
		}
		else{
			p->intercambiar(mayor);
			p = mayor;
		}
	}
}

void MaxHeap::push(Estado* e, int prioridad){
	HeapNode* nuevo = new HeapNode(e, prioridad);
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
	repareFromBottom(nuevo);
}

Estado* MaxHeap::top(){
	return root->getDato();
}

// El dato del ultimo nodo pasa a la raiz, y ese nodo se libera. Si era hijo
// derecho, su padre recupera un lugar y vuelve al principio de la lista.
void MaxHeap::pop(){
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
	if (root != nullptr){
		repareFromTop(root);
	}
}

void MaxHeap::destruir(HeapNode* node){
	if (node != nullptr){
		destruir(node->getLeft());
		destruir(node->getRight());
		delete node;
	}
}

MaxHeap::~MaxHeap(){
	destruir(root);
}
