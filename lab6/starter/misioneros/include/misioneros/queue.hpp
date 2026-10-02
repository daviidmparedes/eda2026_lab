#ifndef MISIONEROS_QUEUE_HPP
#define MISIONEROS_QUEUE_HPP

#include "misioneros/datanode.hpp"
#include "misioneros/stacknode.hpp"

/**
 * ADT Cola (libro, Cap. 5.2). Es la del laboratorio 5, ya implementada.
 *
 * Misma interfaz que Stack. push entra por tail; pop y front trabajan por
 * head: la primera ficha en entrar es la primera en salir (FIFO).
 * ~Queue saca todo lo que quede.
 */
class Queue{
	private:
		StackNode* head;
		StackNode* tail;
	public:
		Queue();
		void push(DataNode* data);
		void pop();
		bool isEmpty();
		StackNode* front();
		~Queue();
};

#endif
