#include "puzzle/heapNode.hpp"

HeapNode::HeapNode(Estado* dato_, int prioridad_):
	dato(dato_), prioridad(prioridad_), left(nullptr), right(nullptr), parent(nullptr){
}

Estado* HeapNode::getDato(){
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
	Estado* d = dato;
	int p = prioridad;
	dato = otro->dato;
	prioridad = otro->prioridad;
	otro->dato = d;
	otro->prioridad = p;
}

bool HeapNode::isLeft(){
	return parent != nullptr && parent->getLeft() == this;
}
