#include "busqueda/queue.hpp"

Queue::Queue(): head(nullptr), tail(nullptr){
}

// La ficha entra por el final. Si la cola esta vacia, la caja nueva es a la
// vez la primera y la ultima.
void Queue::push(DataNode* data){
	StackNode* node = new StackNode(data);
	if (tail == nullptr){
		head = node;
		tail = node;
	}
	else{
		tail->setNext(node);
		tail = node;
	}
}

// Igual que el pop de la pila, con un cuidado extra: si la cola queda vacia,
// tail tambien vuelve a nullptr. Si no, tail seguiria apuntando a la caja
// recien borrada y el siguiente push escribiria en memoria liberada.
void Queue::pop(){
	StackNode* ptr = head;
	if (head != nullptr){
		head = head->getNext();
		delete ptr;
		if (head == nullptr){
			tail = nullptr;
		}
	}
}

StackNode* Queue::front(){
	return head;
}

bool Queue::isEmpty(){
	return (head == nullptr);
}

Queue::~Queue(){
	while (!isEmpty()){
		pop();
	}
}
