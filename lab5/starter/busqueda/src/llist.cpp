#include "busqueda/llist.hpp"
#include <iostream>

LList::LList(): head(nullptr){
}

Node* LList::getHead(){
	return head;
}

void LList::insert(int id){
	Node* node = new Node(id);
	if (head == nullptr){
		head = node;
	}
	else{
		node->setNext(head);
		head = node;
	}
}

void LList::print(){
	Node* ptr = head;
	while (ptr != nullptr){
		std::cout << ptr->getId() << " ";
		ptr = ptr->getNext();
	}
	std::cout << std::endl;
}

// En el codigo de clases este destructor venia vacio, y cada lista dejaba sus
// nodos sin liberar. Aca se recorre la lista borrando nodo por nodo; hay que
// guardar el siguiente ANTES del delete, porque despues ya no se puede leer.
LList::~LList(){
	Node* ptr = head;
	while (ptr != nullptr){
		Node* next = ptr->getNext();
		delete ptr;
		ptr = next;
	}
}
