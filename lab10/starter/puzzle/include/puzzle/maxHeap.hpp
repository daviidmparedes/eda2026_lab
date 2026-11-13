#ifndef PUZZLE_MAXHEAP_HPP
#define PUZZLE_MAXHEAP_HPP

#include "puzzle/estado.hpp"
#include "puzzle/heapNode.hpp"
#include "puzzle/listaNodos.hpp"

/**
 * El heap del laboratorio 9 (libro, Cap. 6.5 y 6.6), adaptado al puzzle:
 *   1. el dato es un Estado*;
 *   2. es de maximo: en la raiz queda el estado de MAYOR prioridad.
 *
 * Tiene las mismas operaciones que la Cola, salvo que push recibe ademas la
 * prioridad. Guarda punteros: pop saca el estado del heap, pero no lo libera.
 * Con prioridades iguales, cualquiera puede salir antes.
 */
class MaxHeap{
	private:
		HeapNode* root;
		// Los nodos con lugar para un hijo. El primero es el padre del proximo
		// nodo, y el ultimo es el ultimo nodo insertado.
		ListaNodos padres;
		void repareFromBottom(HeapNode* node);
		void repareFromTop(HeapNode* node);
		void destruir(HeapNode* node);
	public:
		MaxHeap();
		bool isEmpty();
		void push(Estado* e, int prioridad);
		// El estado de mayor prioridad. El heap no esta vacio.
		Estado* top();
		void pop();
		~MaxHeap();
};

#endif
