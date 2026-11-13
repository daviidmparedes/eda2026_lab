#include "avl/avl.hpp"

// ===========================================================================
// SOLUCION DE REFERENCIA
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
	AVLNode* x = node->getRight();
	AVLNode* b = x->getLeft();
	AVLNode* parent = node->getParent();

	// B pasa de X a P.
	node->setRight(b);
	if (b != nullptr){
		b->setParent(node);
	}

	// X ocupa el lugar de P: bajo el padre de P, o como raiz.
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

	// P queda bajo X. Recien aqui, porque isLeft() de P lee a su padre.
	x->setLeft(node);
	node->setParent(x);

	// Primero P, que ahora esta abajo, y despues X, que depende de P.
	actualizarAltura(node);
	actualizarAltura(x);
}

// Rotacion Right (Figura 6.19): la misma, cambiando izquierda por derecha.
void AVL::rightRotation(AVLNode* node){
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

// Los cuatro casos del libro (6.4.1). El signo del factor de node dice de que
// lado sobra altura; el del hijo de ese lado dice si la rama pesada va
// derecho (rotacion simple) o dobla (rotacion doble: primero se endereza la
// rama rotando el hijo, y despues se rota node).
void AVL::balance(AVLNode* node){
	if (factorBalance(node) > 1){
		if (factorBalance(node->getLeft()) < 0){
			leftRotation(node->getLeft());      // Left-Right
		}
		rightRotation(node);                     // Right
	}
	else if (factorBalance(node) < -1){
		if (factorBalance(node->getRight()) > 0){
			rightRotation(node->getRight());    // Right-Left
		}
		leftRotation(node);                      // Left
	}
}
