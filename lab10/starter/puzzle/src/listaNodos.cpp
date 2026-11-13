#include "puzzle/listaNodos.hpp"

ListaNodos::ListaNodos(): head(nullptr), tail(nullptr){
}

bool ListaNodos::isEmpty(){
	return head == nullptr;
}

HeapNode* ListaNodos::front(){
	return head->nodo;
}

HeapNode* ListaNodos::back(){
	return tail->nodo;
}

void ListaNodos::pushFront(HeapNode* node){
	Caja* c = new Caja{node, nullptr, head};
	if (head == nullptr){
		tail = c;
	}
	else{
		head->prev = c;
	}
	head = c;
}

void ListaNodos::pushBack(HeapNode* node){
	Caja* c = new Caja{node, tail, nullptr};
	if (tail == nullptr){
		head = c;
	}
	else{
		tail->next = c;
	}
	tail = c;
}

void ListaNodos::popFront(){
	Caja* c = head;
	head = head->next;
	if (head == nullptr){
		tail = nullptr;
	}
	else{
		head->prev = nullptr;
	}
	delete c;
}

void ListaNodos::popBack(){
	Caja* c = tail;
	tail = tail->prev;
	if (tail == nullptr){
		head = nullptr;
	}
	else{
		tail->next = nullptr;
	}
	delete c;
}

ListaNodos::~ListaNodos(){
	while (!isEmpty()){
		popFront();
	}
}
