#ifndef BUSQUEDA_STACKNODE_HPP
#define BUSQUEDA_STACKNODE_HPP

#include "busqueda/datanode.hpp"

/**
 * La caja de la pila: un puntero a la ficha (DataNode) y otro a la caja que
 * sigue. La cola usa esta misma caja.
 *
 * Caja y ficha son cosas distintas. pop() siempre bota la caja; la ficha solo
 * se libera junto con ella si no tiene hijas, porque si las tiene es padre de
 * alguien y el camino la va a necesitar.
 */
class StackNode{
	private:
		DataNode* data;
		StackNode* next;
	public:
		StackNode();
		StackNode(DataNode* data_);
		DataNode* getData();
		StackNode* getNext();
		void setData(DataNode* data_);
		void setNext(StackNode* next_);
		~StackNode();
};

#endif
