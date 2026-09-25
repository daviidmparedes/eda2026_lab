#include "busqueda/node.hpp"

Node::Node(): id(-1), next(nullptr){
}

Node::Node(int id_, Node* next_): id(id_), next(next_){
}

void Node::setId(int id_){
	id = id_;
}

void Node::setNext(Node* next_){
	next = next_;
}

int Node::getId(){
	return id;
}

Node* Node::getNext(){
	return next;
}

Node::~Node(){
}
