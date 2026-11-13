#ifndef AVL_AVLNODE_HPP
#define AVL_AVLNODE_HPP

/**
 * Nodo de un arbol AVL (libro, Cap. 6.4.1). Es el ABBNode del laboratorio 8
 * con un campo mas, la altura.
 *
 *   data     la llave
 *   altura   la altura del subarbol que parte en este nodo (una hoja: 0)
 *   left     el hijo izquierdo (nullptr si no tiene)
 *   right    el hijo derecho (nullptr si no tiene)
 *   parent   el padre (nullptr en la raiz)
 *
 * Igual que en el laboratorio 8:
 *   - setLeft, setRight y setParent cambian un solo puntero. setLeft(x) no
 *     cambia el padre de x: eso se hace aparte, con x->setParent(...).
 *   - setAltura solo guarda el numero que se le da: no lo calcula.
 */
class AVLNode{
	private:
		int data;
		int altura;
		AVLNode* left;
		AVLNode* right;
		AVLNode* parent;
	public:
		AVLNode(int data_, AVLNode* parent_ = nullptr);
		int getData();
		int getAltura();
		AVLNode* getLeft();
		AVLNode* getRight();
		AVLNode* getParent();
		void setData(int data_);
		void setAltura(int altura_);
		void setLeft(AVLNode* node);
		void setRight(AVLNode* node);
		void setParent(AVLNode* node);
		// true si este nodo es el hijo izquierdo de su padre.
		bool isLeft();
		// true si este nodo es el hijo derecho de su padre.
		bool isRight();
};

#endif
