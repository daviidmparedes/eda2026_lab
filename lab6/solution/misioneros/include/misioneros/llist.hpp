#ifndef MISIONEROS_LLIST_HPP
#define MISIONEROS_LLIST_HPP

#include "misioneros/node.hpp"

/**
 * Lista enlazada de enteros (libro, Cap. 4.2). Es la misma del laboratorio 5.
 *
 * insert() inserta al inicio:
 *
 *     L.insert(1);  L.insert(2);      ->   L queda  2 -> 1
 */
class LList{
	private:
		Node* head;
	public:
		LList();
		Node* getHead();
		void insert(int id);
		void print();
		// Libera todos los nodos de la lista.
		virtual ~LList();
};

#endif
