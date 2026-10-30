#ifndef AVL_AVL_HPP
#define AVL_AVL_HPP

#include "avl/avlNode.hpp"

/**
 * Laboratorio 9 - Arbol AVL (libro, Cap. 6.4)
 *
 * Un AVL es un ABB balanceado: en cada nodo, la altura del subarbol izquierdo
 * y la del derecho difieren a lo mas en 1. Despues de insertar, si un nodo
 * queda con una diferencia de 2, se arregla con rotaciones.
 *
 * El factor de balance de un nodo es altura(izquierdo) - altura(derecho):
 *   +2  sobra altura a la izquierda
 *   -2  sobra altura a la derecha
 */
class AVL{
	private:
		AVLNode* root;
		bool insertRec(int key, AVLNode* node);
		void destruir(AVLNode* node);

	public:
		// ---------------------------------------------------------------
		// Dado (src/avl_dado.cpp)
		// ---------------------------------------------------------------
		AVL();
		~AVL();
		AVLNode* getRoot();
		// Cambia la raiz. Lo usa ./test para armar arboles sin pasar por
		// insert.
		void setRoot(AVLNode* node);
		// Dibuja el arbol en la pantalla.
		void imprimir();

		// La altura de node, o -1 si node es nullptr.
		static int altura(AVLNode* node);
		// Guarda en node la altura que le corresponde segun sus hijos:
		// 1 + la mayor de las dos. Supone que las de sus hijos estan al dia.
		static void actualizarAltura(AVLNode* node);
		// altura(hijo izquierdo) - altura(hijo derecho).
		static int factorBalance(AVLNode* node);

		// Inserta key como en un ABB (sin repetidas). De vuelta hacia la
		// raiz, actualiza la altura de cada nodo del camino y, si alguno
		// queda con factor de balance 2 o -2, llama a balance sobre el.
		// Retorna false si key ya estaba.
		bool insert(int key);

		// ---------------------------------------------------------------
		// Parte 1 - Rotaciones simples
		// ---------------------------------------------------------------

		// Rotacion Left (libro, Figura 6.18). node tiene hijo derecho X. X
		// pasa a ocupar el lugar de node, node queda como hijo izquierdo de
		// X, y el que era el hijo izquierdo de X queda como hijo derecho de
		// node. Deja al dia los padres de los nodos que se movieron, el
		// hijo del padre de node (o root, si node era la raiz) y la altura
		// de node y de X.
		void leftRotation(AVLNode* node);

		// Rotacion Right (libro, Figura 6.19): la misma, al reves. node
		// tiene hijo izquierdo X; X sube, node queda como su hijo derecho, y
		// el que era el hijo derecho de X queda como hijo izquierdo de node.
		void rightRotation(AVLNode* node);

		// ---------------------------------------------------------------
		// Parte 2 - Balancear
		// ---------------------------------------------------------------

		// node tiene factor de balance 2 o -2. Aplica la rotacion que
		// corresponde (Left, Right, Left-Right o Right-Left) para que el
		// subarbol que partia en node quede balanceado.
		void balance(AVLNode* node);
};

#endif
