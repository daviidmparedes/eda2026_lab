#include "heap/heap.hpp"

// ===========================================================================
// Laboratorio 9 - AQUI TRABAJAS TU (parte Heap).
//
// Lo que debe hacer cada funcion esta descrito en include/heap/heap.hpp.
// ===========================================================================

// ---------------------------------------------------------------------------
// Parte 1 - Reparar
// ---------------------------------------------------------------------------

void Heap::repareFromBottom(HeapNode* node){
	// TODO: implementar.
	HeapNode* p = node;
	while (p->getParent() != nullptr && p->getPrioridad() < p->getParent()->getPrioridad()){
		p->intercambiar(p->getParent());
		p = p->getParent();
	}
}

void Heap::repareFromTop(HeapNode* node){
	// TODO: implementar.
	HeapNode* p = node;
	bool listo = false;
	while(!listo){
		HeapNode* menor = p;
		if(p->getLeft() != nullptr && p->getLeft()->getPrioridad() < menor->getPrioridad()){
			menor = p->getLeft();
		}
		if(p->getRight() != nullptr && p->getRight()->getPrioridad() < menor->getPrioridad()){
			menor = p->getRight();
		}
		if (menor == p){
			listo = true;
		}else{
			p->intercambiar(menor);
			p=menor;
		}
	}
}

// ---------------------------------------------------------------------------
// Parte 2 - push y top
// ---------------------------------------------------------------------------

void Heap::push(int dato, int prioridad){
	// TODO: implementar.
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
	// TODO: implementar.
	return root->getDato();
}

int Heap::topPrioridad(){
	// TODO: implementar.
	return root->getPrioridad();
}

// ---------------------------------------------------------------------------
// Parte 3 - pop
// ---------------------------------------------------------------------------

void Heap::pop(){
	// TODO: implementar.
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
