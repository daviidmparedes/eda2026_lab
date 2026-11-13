#include "puzzle/cola.hpp"

Cola::Cola(): head(nullptr), tail(nullptr){
}

bool Cola::isEmpty(){
	return head == nullptr;
}

void Cola::push(Estado* e){
	Caja* c = new Caja{e, nullptr};
	if (head == nullptr){
		head = c;
	}
	else{
		tail->next = c;
	}
	tail = c;
}

Estado* Cola::top(){
	return head->estado;
}

void Cola::pop(){
	Caja* c = head;
	head = head->next;
	delete c;
}

Cola::~Cola(){
	while (!isEmpty()){
		pop();
	}
}
