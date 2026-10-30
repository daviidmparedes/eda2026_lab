#include "heap/listaNodos.hpp"

ListaNodos::ListaNodos(): head(nullptr), tail(nullptr), n(0){
}

bool ListaNodos::isEmpty(){
	return head == nullptr;
}

int ListaNodos::size(){
	return n;
}

HeapNode* ListaNodos::front(){
	return head == nullptr ? nullptr : head->nodo;
}

HeapNode* ListaNodos::back(){
	return tail == nullptr ? nullptr : tail->nodo;
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
	n = n + 1;
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
	n = n + 1;
}

void ListaNodos::popFront(){
	if (head != nullptr){
		Caja* c = head;
		head = head->next;
		if (head == nullptr){
			tail = nullptr;
		}
		else{
			head->prev = nullptr;
		}
		delete c;
		n = n - 1;
	}
}

void ListaNodos::popBack(){
	if (tail != nullptr){
		Caja* c = tail;
		tail = tail->prev;
		if (tail == nullptr){
			head = nullptr;
		}
		else{
			tail->next = nullptr;
		}
		delete c;
		n = n - 1;
	}
}

ListaNodos::~ListaNodos(){
	while (!isEmpty()){
		popFront();
	}
}
