#ifndef MISIONEROS_STACKNODE_HPP
#define MISIONEROS_STACKNODE_HPP

#include "misioneros/datanode.hpp"

/**
 * Caja de la pila y de la cola: un puntero a la ficha y otro a la caja
 * siguiente. Es la misma del laboratorio 5.
 *
 * ~StackNode libera la ficha si su nchildren es 0.
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
