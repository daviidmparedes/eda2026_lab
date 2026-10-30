#ifndef ABB_ABB_HPP
#define ABB_ABB_HPP

#include "abb/abbNode.hpp"

/**
 * Laboratorio 8 - Arbol binario de busqueda, ABB (libro, Cap. 6.3)
 *
 * En un ABB, todas las llaves del subarbol izquierdo de un nodo son menores
 * que la llave del nodo, y todas las del subarbol derecho son mayores. Este
 * arbol no guarda llaves repetidas.
 *
 * En cada nodo, el puntero al padre tiene que estar siempre al dia: la raiz
 * no tiene padre, y el padre de cualquier otro nodo es el nodo que lo tiene
 * como hijo.
 */
class ABB{
	private:
		ABBNode* root;

		// Versiones recursivas de insert, find e inOrder, que trabajan sobre
		// el subarbol que parte en node.
		bool insertRec(int key, ABBNode* node);
		ABBNode* findRec(int key, ABBNode* node);
		int inOrderRec(ABBNode* node, int* keys, int k);

		// Dado: libera todos los nodos del subarbol que parte en node.
		void destruir(ABBNode* node);

	public:
		// ---------------------------------------------------------------
		// Dado (src/abb_dado.cpp)
		// ---------------------------------------------------------------
		ABB();
		~ABB();
		ABBNode* getRoot();
		// Cambia la raiz. Lo usa ./test para armar arboles sin pasar por
		// insert.
		void setRoot(ABBNode* node);
		// Dibuja el arbol en la pantalla.
		void imprimir();

		// ---------------------------------------------------------------
		// Parte 1 - Insertar, buscar y recorrer
		// ---------------------------------------------------------------

		// Inserta key como una hoja nueva, en el lugar que le corresponde.
		// Retorna true si la inserto, o false si key ya estaba en el arbol
		// (y entonces el arbol no cambia).
		bool insert(int key);

		// El nodo que tiene la llave key, o nullptr si key no esta en el
		// arbol (tambien si el arbol esta vacio).
		ABBNode* find(int key);

		// Escribe en keys todas las llaves del arbol en recorrido in-orden,
		// y retorna cuantas escribio. keys tiene espacio para todas.
		int inOrder(int* keys);

		// ---------------------------------------------------------------
		// Parte 2 - El menor y el mayor
		// ---------------------------------------------------------------

		// El nodo con la llave mas chica del subarbol que parte en node.
		ABBNode* getLowest(ABBNode* node);

		// El nodo con la llave mas grande del subarbol que parte en node.
		ABBNode* getHighest(ABBNode* node);

		// ---------------------------------------------------------------
		// Parte 3 - Eliminar
		// ---------------------------------------------------------------

		// Saca del arbol a node, que tiene a lo mas un hijo, y lo libera. Su
		// hijo, si tiene, queda en su lugar: bajo el padre de node, o como
		// raiz si node era la raiz.
		void updateForRemove(ABBNode* node);

		// Saca del arbol a node, un nodo que ya se encontro con find, y
		// libera la memoria de un nodo. El arbol sigue siendo un ABB, con
		// todas las demas llaves. Si node es nullptr, no hace nada.
		void remove(ABBNode* node);
};

#endif
