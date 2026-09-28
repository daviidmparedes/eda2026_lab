#ifndef BUSQUEDA_STACK_HPP
#define BUSQUEDA_STACK_HPP

#include "busqueda/datanode.hpp"
#include "busqueda/stacknode.hpp"

/**
 * ADT Pila (libro, Cap. 5.1), la del codigo visto en clases.
 *
 * push y pop trabajan solo por head: push es el insertFirst de una lista y
 * pop es su removeFirst. Por eso la ultima ficha en entrar es la primera en
 * salir (LIFO).
 *
 * El libro llama top() a la consulta; este codigo la llama front().
 */
class Stack{
	private:
		StackNode* head;
	public:
		Stack();
		void push(DataNode* data);
		void pop();
		bool isEmpty();
		StackNode* front();
		// Saca todo lo que quede, asi que las fichas se liberan solas cuando
		// la pila deja de existir.
		~Stack();
};

#endif
