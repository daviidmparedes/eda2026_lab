#include "heap/heapNode.hpp"
#include <set>

// Los nodos creados y todavia no liberados. Vive dentro de una funcion para
// que el conjunto ya exista cuando se crea el primer nodo.
static std::set<HeapNode*>& vivos(){
	static std::set<HeapNode*> conjunto;
	return conjunto;
}

HeapNode::HeapNode(int dato_, int prioridad_, HeapNode* parent_):
	dato(dato_), prioridad(prioridad_), left(nullptr), right(nullptr), parent(parent_){
	vivos().insert(this);
}

int HeapNode::getDato(){
	return dato;
}

int HeapNode::getPrioridad(){
	return prioridad;
}

HeapNode* HeapNode::getLeft(){
	return left;
}

HeapNode* HeapNode::getRight(){
	return right;
}

HeapNode* HeapNode::getParent(){
	return parent;
}

void HeapNode::setLeft(HeapNode* node){
	left = node;
}

void HeapNode::setRight(HeapNode* node){
	right = node;
}

void HeapNode::setParent(HeapNode* node){
	parent = node;
}

void HeapNode::intercambiar(HeapNode* otro){
	int d = dato;
	int p = prioridad;
	dato = otro->dato;
	prioridad = otro->prioridad;
	otro->dato = d;
	otro->prioridad = p;
}

// La raiz no tiene padre, asi que no es hijo izquierdo ni derecho.
bool HeapNode::isLeft(){
	return parent != nullptr && parent->getLeft() == this;
}

bool HeapNode::isRight(){
	return parent != nullptr && parent->getRight() == this;
}

int HeapNode::nodosVivos(){
	return vivos().size();
}

bool HeapNode::estaVivo(HeapNode* node){
	return vivos().count(node) > 0;
}

HeapNode::~HeapNode(){
	vivos().erase(this);
}
