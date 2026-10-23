#ifndef ABB_ABBNODE_HPP
#define ABB_ABBNODE_HPP

/**
 * Nodo de un arbol binario de busqueda (libro, Cap. 6.3.2).
 *
 *   data     la llave
 *   left     el hijo izquierdo (nullptr si no tiene)
 *   right    el hijo derecho (nullptr si no tiene)
 *   parent   el padre (nullptr en la raiz)
 *
 * Dos detalles:
 *   - setLeft, setRight y setParent cambian un solo puntero. setLeft(x) no
 *     cambia el padre de x: eso se hace aparte, con x->setParent(...).
 *   - ~ABBNode libera solo este nodo: no toca a sus hijos ni a su padre.
 */
class ABBNode{
	private:
		int data;
		ABBNode* left;
		ABBNode* right;
		ABBNode* parent;
	public:
		ABBNode(int data_, ABBNode* parent_ = nullptr);
		int getData();
		ABBNode* getLeft();
		ABBNode* getRight();
		ABBNode* getParent();
		void setData(int data_);
		void setLeft(ABBNode* node);
		void setRight(ABBNode* node);
		void setParent(ABBNode* node);
		// true si este nodo es el hijo izquierdo de su padre.
		bool isLeft();
		// true si este nodo es el hijo derecho de su padre.
		bool isRight();
		// Cuantos ABBNode existen en este momento (creados y no liberados).
		static int nodosVivos();
		// true si node es un ABBNode creado y todavia no liberado. La usan
		// ./test y el destructor del arbol para no seguir punteros a nodos
		// que ya se liberaron.
		static bool estaVivo(ABBNode* node);
		~ABBNode();
};

#endif
