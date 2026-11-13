#ifndef PUZZLE_LISTANODOS_HPP
#define PUZZLE_LISTANODOS_HPP

#include "puzzle/heapNode.hpp"

/**
 * La lista doblemente enlazada del laboratorio 9, sin cambios: se puede
 * agregar y sacar por los dos extremos. Agregar o sacar un nodo de la lista
 * no crea ni libera ese HeapNode.
 */
class ListaNodos{
	private:
		struct Caja{
			HeapNode* nodo;
			Caja* prev;
			Caja* next;
		};
		Caja* head;
		Caja* tail;
	public:
		ListaNodos();
		bool isEmpty();
		// El primero y el ultimo de la lista. La lista no esta vacia.
		HeapNode* front();
		HeapNode* back();
		void pushFront(HeapNode* node);
		void pushBack(HeapNode* node);
		void popFront();
		void popBack();
		~ListaNodos();
};

#endif
