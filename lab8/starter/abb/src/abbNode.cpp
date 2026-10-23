#include "abb/abbNode.hpp"
#include <set>

// Los nodos creados y todavia no liberados. Vive dentro de una funcion para
// que el conjunto ya exista cuando se crea el primer nodo.
static std::set<ABBNode*>& vivos(){
	static std::set<ABBNode*> conjunto;
	return conjunto;
}

ABBNode::ABBNode(int data_, ABBNode* parent_):
	data(data_), left(nullptr), right(nullptr), parent(parent_){
	vivos().insert(this);
}

int ABBNode::getData(){
	return data;
}

ABBNode* ABBNode::getLeft(){
	return left;
}

ABBNode* ABBNode::getRight(){
	return right;
}

ABBNode* ABBNode::getParent(){
	return parent;
}

void ABBNode::setData(int data_){
	data = data_;
}

void ABBNode::setLeft(ABBNode* node){
	left = node;
}

void ABBNode::setRight(ABBNode* node){
	right = node;
}

void ABBNode::setParent(ABBNode* node){
	parent = node;
}

// La raiz no tiene padre, asi que no es hijo izquierdo ni derecho.
bool ABBNode::isLeft(){
	return parent != nullptr && parent->getLeft() == this;
}

bool ABBNode::isRight(){
	return parent != nullptr && parent->getRight() == this;
}

int ABBNode::nodosVivos(){
	return vivos().size();
}

bool ABBNode::estaVivo(ABBNode* node){
	return vivos().count(node) > 0;
}

ABBNode::~ABBNode(){
	vivos().erase(this);
}
