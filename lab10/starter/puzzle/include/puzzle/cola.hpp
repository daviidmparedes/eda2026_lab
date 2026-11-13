#ifndef PUZZLE_COLA_HPP
#define PUZZLE_COLA_HPP

#include "puzzle/estado.hpp"

/**
 * Cola FIFO de estados (libro, Cap. 4). Guarda punteros: pop saca el estado
 * de la cola, pero no lo libera.
 */
class Cola{
	private:
		struct Caja{
			Estado* estado;
			Caja* next;
		};
		Caja* head;
		Caja* tail;
	public:
		Cola();
		bool isEmpty();
		void push(Estado* e);
		// El primero que entro. La cola no esta vacia.
		Estado* top();
		void pop();
		~Cola();
};

#endif
