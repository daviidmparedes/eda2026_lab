#ifndef HEAP_LISTANODOS_HPP
#define HEAP_LISTANODOS_HPP

#include "heap/heapNode.hpp"

/**
 * Lista doblemente enlazada de punteros a HeapNode (libro, Cap. 4.4). Se
 * puede agregar y sacar por los dos extremos, y cada operacion cuesta O(1).
 *
 * La lista guarda punteros: agregar o sacar un nodo de la lista no crea ni
 * libera ese HeapNode. ~ListaNodos libera solo las cajas de la lista.
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
		int n;
	public:
		ListaNodos();
		bool isEmpty();
		int size();
		// El primero y el ultimo de la lista (nullptr si esta vacia).
		HeapNode* front();
		HeapNode* back();
		// Agregan node al principio o al final.
		void pushFront(HeapNode* node);
		void pushBack(HeapNode* node);
		// Sacan el primero o el ultimo. Si la lista esta vacia, no hacen nada.
		void popFront();
		void popBack();
		~ListaNodos();
};

#endif
