#ifndef HEAP_HEAP_HPP
#define HEAP_HEAP_HPP

#include "heap/heapNode.hpp"
#include "heap/listaNodos.hpp"

/**
 * Laboratorio 9 - Heap (libro, Cap. 6.5 y 6.6)
 *
 * Un heap es un arbol binario que cumple dos propiedades:
 *   1. es completo: se llena por niveles, y cada nivel de izquierda a
 *      derecha;
 *   2. ningun nodo tiene una prioridad menor que la de su padre.
 *
 * El libro trabaja con un Max-Heap. Aqui el heap es de minimo (el libro, en
 * 6.5, dice que tambien se puede): en la raiz queda siempre el dato con la
 * prioridad mas chica. Con prioridades iguales, cualquiera puede salir antes.
 *
 * Para saber donde va el proximo nodo, el heap lleva la lista padres: los
 * nodos que todavia tienen lugar para un hijo, en el orden en que llegaron.
 * El primero de la lista es el padre del proximo nodo que se inserte, y el
 * ultimo es el ultimo nodo insertado.
 */
class Heap{
	private:
		HeapNode* root;
		int n;
		ListaNodos padres;
		void destruir(HeapNode* node);

	public:
		// ---------------------------------------------------------------
		// Dado (src/heap_dado.cpp)
		// ---------------------------------------------------------------
		Heap();
		~Heap();
		HeapNode* getRoot();
		// Cambia la raiz. Lo usa ./test para armar arboles sin pasar por
		// push: no cambia n ni la lista padres.
		void setRoot(HeapNode* node);
		// Cuantos datos hay en el heap.
		int size();
		bool isEmpty();
		// Dibuja el arbol en la pantalla, con las prioridades.
		void imprimir();

		// ---------------------------------------------------------------
		// Parte 1 - Reparar
		// ---------------------------------------------------------------

		// Repara el heap desde node hacia arriba (libro, 6.6.0.2): mientras
		// la prioridad de node sea menor que la de su padre, intercambia sus
		// datos y sigue desde el padre.
		void repareFromBottom(HeapNode* node);

		// Repara el heap desde node hacia abajo (libro, 6.6.2): mientras
		// algun hijo de node tenga una prioridad menor que la de node,
		// intercambia node con el hijo de menor prioridad y sigue desde ese
		// hijo.
		void repareFromTop(HeapNode* node);

		// ---------------------------------------------------------------
		// Parte 2 - push y top
		// ---------------------------------------------------------------

		// Agrega dato con su prioridad en un nodo nuevo, en el primer lugar
		// libre del arbol (bajo el primero de padres), y repara el heap.
		// Deja al dia n y la lista padres.
		void push(int dato, int prioridad);

		// El dato de la raiz, y su prioridad. El heap no esta vacio.
		int top();
		int topPrioridad();

		// ---------------------------------------------------------------
		// Parte 3 - pop
		// ---------------------------------------------------------------

		// Saca el dato de la raiz (libro, 6.6.2): el dato del ultimo nodo
		// insertado pasa a la raiz, ese nodo se libera y el heap se repara
		// desde la raiz. Deja al dia n y la lista padres. Si el heap esta
		// vacio, no hace nada.
		void pop();
};

#endif
