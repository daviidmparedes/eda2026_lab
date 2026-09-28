#ifndef BUSQUEDA_LLIST_HPP
#define BUSQUEDA_LLIST_HPP

#include "busqueda/node.hpp"

/**
 * Lista enlazada reducida (libro, Cap. 4.2): la del codigo visto en clases.
 *
 * insert() inserta AL INICIO (es el insertFirst del libro), asi que la lista
 * se recorre al reves de como se inserto:
 *
 *     L.insert(1);  L.insert(2);      ->   L queda  2 -> 1
 *
 * Esa misma propiedad sirve al reconstruir un camino: si se inserta mientras
 * se retrocede desde la meta hasta el inicio, la lista queda en orden, desde
 * el inicio hasta la meta, sin ningun paso extra.
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
