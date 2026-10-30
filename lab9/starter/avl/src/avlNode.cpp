#include "avl/avlNode.hpp"

AVLNode::AVLNode(int data_, AVLNode* parent_):
	data(data_), altura(0), left(nullptr), right(nullptr), parent(parent_){
}

int AVLNode::getData(){
	return data;
}

int AVLNode::getAltura(){
	return altura;
}

AVLNode* AVLNode::getLeft(){
	return left;
}

AVLNode* AVLNode::getRight(){
	return right;
}

AVLNode* AVLNode::getParent(){
	return parent;
}

void AVLNode::setData(int data_){
	data = data_;
}

void AVLNode::setAltura(int altura_){
	altura = altura_;
}

void AVLNode::setLeft(AVLNode* node){
	left = node;
}

void AVLNode::setRight(AVLNode* node){
	right = node;
}

void AVLNode::setParent(AVLNode* node){
	parent = node;
}

// La raiz no tiene padre, asi que no es hijo izquierdo ni derecho.
bool AVLNode::isLeft(){
	return parent != nullptr && parent->getLeft() == this;
}

bool AVLNode::isRight(){
	return parent != nullptr && parent->getRight() == this;
}
