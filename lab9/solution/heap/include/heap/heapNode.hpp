#ifndef HEAP_HEAPNODE_HPP
#define HEAP_HEAPNODE_HPP

/**
 * Nodo de un heap (libro, Cap. 6.5 y 6.6).
 *
 *   dato       lo que se guarda (por ejemplo, un id)
 *   prioridad  el numero por el que se ordena el heap
 *   left       el hijo izquierdo (nullptr si no tiene)
 *   right      el hijo derecho (nullptr si no tiene)
 *   parent     el padre (nullptr en la raiz)
 *
 * Igual que en el laboratorio 8:
 *   - setLeft, setRight y setParent cambian un solo puntero. setLeft(x) no
 *     cambia el padre de x: eso se hace aparte, con x->setParent(...).
 *   - ~HeapNode libera solo este nodo: no toca a sus hijos ni a su padre.
 */
class HeapNode{
	private:
		int dato;
		int prioridad;
		HeapNode* left;
		HeapNode* right;
		HeapNode* parent;
	public:
		HeapNode(int dato_, int prioridad_, HeapNode* parent_ = nullptr);
		int getDato();
		int getPrioridad();
		HeapNode* getLeft();
		HeapNode* getRight();
		HeapNode* getParent();
		void setLeft(HeapNode* node);
		void setRight(HeapNode* node);
		void setParent(HeapNode* node);
		// Intercambia el dato y la prioridad de este nodo con los de otro.
		// Los punteros de los dos nodos no cambian (libro, 6.6: "la funcion de
		// intercambio cambiara el DATO de los nodos y no los punteros").
		void intercambiar(HeapNode* otro);
		// true si este nodo es el hijo izquierdo de su padre.
		bool isLeft();
		// true si este nodo es el hijo derecho de su padre.
		bool isRight();
		// Cuantos HeapNode existen en este momento (creados y no liberados).
		static int nodosVivos();
		// true si node es un HeapNode creado y todavia no liberado. La usan
		// ./test y el destructor del heap para no seguir punteros a nodos que
		// ya se liberaron.
		static bool estaVivo(HeapNode* node);
		~HeapNode();
};

#endif
