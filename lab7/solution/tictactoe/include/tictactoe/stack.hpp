#ifndef TICTACTOE_STACK_HPP
#define TICTACTOE_STACK_HPP

#include "tictactoe/datanode.hpp"
#include "tictactoe/stacknode.hpp"

/**
 * ADT Pila (libro, Cap. 5.1). Es la misma del laboratorio 5.
 *
 * push y pop trabajan por head: la ultima ficha en entrar es la primera en
 * salir (LIFO). ~Stack saca todo lo que quede.
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
		~Stack();
};

#endif
