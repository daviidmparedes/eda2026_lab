#ifndef PUZZLE_HEAPNODE_HPP
#define PUZZLE_HEAPNODE_HPP

#include "puzzle/estado.hpp"

/**
 * Nodo del heap del laboratorio 9, adaptado: el dato ahora es un Estado*.
 */
class HeapNode{
	private:
		Estado* dato;
		int prioridad;
		HeapNode* left;
		HeapNode* right;
		HeapNode* parent;
	public:
		HeapNode(Estado* dato_, int prioridad_);
		Estado* getDato();
		int getPrioridad();
		HeapNode* getLeft();
		HeapNode* getRight();
		HeapNode* getParent();
		void setLeft(HeapNode* node);
		void setRight(HeapNode* node);
		void setParent(HeapNode* node);
		// Intercambia el dato y la prioridad con los de otro. Los punteros de
		// los dos nodos no cambian.
		void intercambiar(HeapNode* otro);
		// true si este nodo es el hijo izquierdo de su padre.
		bool isLeft();
};

#endif
