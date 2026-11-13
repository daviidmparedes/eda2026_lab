#include "avl/avl.hpp"

// ===========================================================================
// Laboratorio 9 - AQUI TRABAJAS TU (parte AVL).
//
// Lo que debe hacer cada funcion esta descrito en include/avl/avl.hpp.
// ===========================================================================

// ---------------------------------------------------------------------------
// Parte 1 - Rotaciones simples
// ---------------------------------------------------------------------------

/* Rotacion Left (Figura 6.18). Con P = node, X = su hijo derecho y B = el
   hijo izquierdo de X:

          P                    X
         / \                  / \
        A   X       ->       P   C
           / \              / \
          B   C            A   B

   Cambian seis punteros (tres hijos y tres padres) y dos alturas. A y C no
   se mueven. */

void AVL::leftRotation(AVLNode* node){
	// TODO: implementar.
	AVLNode* x = node->getRight();
	AVLNode* b = x->getLeft();
	AVLNode* parent = node->getParent();

	// B pasar de X a P
	node->setRight(b);
	if(b!= nullptr){
		b->setParent(node);
	}

	// X ocupar el lugar P: bajar al P y subir a X (como raiz)
	x->setParent(parent);
	if(parent == nullptr){
		root = x;
	}else if(node->isLeft()){
		parent->setLeft(x);
	}else{
		parent->setRight(x);
	}

	// P queda bajo X
	x->setLeft(node);
	node->setParent(x);

	actualizarAltura(node);
	actualizarAltura(x);
}

void AVL::rightRotation(AVLNode* node){
	// TODO: implementar.
	AVLNode* x = node->getLeft();
	AVLNode* b = x->getRight();
	AVLNode* parent = node->getParent();

	node->setLeft(b);
	if (b != nullptr){
		b->setParent(node);
	}

	x->setParent(parent);
	if (parent == nullptr){
		root = x;
	}
	else if (node->isLeft()){
		parent->setLeft(x);
	}
	else{
		parent->setRight(x);
	}

	x->setRight(node);
	node->setParent(x);

	actualizarAltura(node);
	actualizarAltura(x);
}

// ---------------------------------------------------------------------------
// Parte 2 - Balancear
// ---------------------------------------------------------------------------

void AVL::balance(AVLNode* node){
	// TODO: implementar.
	if (factorBalance(node) > 1){
		if(factorBalance(node->getLeft()) < 0){
			leftRotation(node->getLeft());
		}
		rightRotation(node);
	}else if (factorBalance(node) < -1){
		if (factorBalance(node->getRight()) > 0){
			rightRotation(node->getRight());    // Right-Left
		}
		leftRotation(node);                      // Left
	}
}
