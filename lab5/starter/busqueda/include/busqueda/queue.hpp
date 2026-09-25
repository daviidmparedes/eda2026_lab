#ifndef BUSQUEDA_QUEUE_HPP
#define BUSQUEDA_QUEUE_HPP

#include "busqueda/datanode.hpp"
#include "busqueda/stacknode.hpp"

/**
 * ADT Cola (libro, Cap. 5.2).
 *
 * Tiene la MISMA interfaz que Stack (push, pop, front, isEmpty) y usa la misma
 * caja (StackNode). La unica diferencia es por donde entra cada ficha:
 *
 *     Stack::push  entra por head  ->  la ultima en entrar sale primero (LIFO)
 *     Queue::push  entra por tail  ->  la primera en entrar sale primero (FIFO)
 *
 * pop y front trabajan por head, igual que en la pila. Para que push sea
 * O(1) hace falta el segundo puntero, tail, que apunta a la ultima caja.
 *
 * Como la interfaz es la misma, pasar una busqueda de pila a cola es cambiar
 * una sola palabra: Stack por Queue.
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
		// Saca todo lo que quede, igual que ~Stack.
		~Queue();
};

#endif
