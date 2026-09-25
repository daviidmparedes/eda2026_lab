#include "busqueda/stack.hpp"

Stack::Stack(): head(nullptr){
}

void Stack::push(DataNode* data){
	StackNode* node = new StackNode(data);
	if (head == nullptr){
		head = node;
	}
	else{
		node->setNext(head);
		head = node;
	}
}

void Stack::pop(){
	StackNode* ptr = head;
	if (head != nullptr){
		head = head->getNext();
		delete ptr;
	}
}

StackNode* Stack::front(){
	return head;
}

bool Stack::isEmpty(){
	return (head == nullptr);
}

// En el codigo de clases este destructor venia vacio y habia que vaciar la
// pila a mano antes de salir de la funcion. Ahora se vacia sola.
Stack::~Stack(){
	while (!isEmpty()){
		pop();
	}
}
