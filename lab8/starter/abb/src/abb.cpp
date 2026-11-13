#include "abb/abb.hpp"

// ===========================================================================
// Laboratorio 8 - AQUI TRABAJAS TU.
//
// Lo que debe hacer cada funcion esta descrito en include/abb/abb.hpp.
// ===========================================================================

// ---------------------------------------------------------------------------
// Parte 1 - Insertar, buscar y recorrer
// ---------------------------------------------------------------------------

bool ABB::insertRec(int key, ABBNode* node){
	// TODO: implementar.
	bool insertado = false;
	if(key < node->getData()){
		if(node->getLeft() == nullptr){
			node->setLeft(new ABBNode(key, node));
			insertado = true;
		}else{
			insertado = insertRec(key, node->getLeft());
		}
	}else if(key > node->getData()){
		if(node->getRight() == nullptr){
			node->setRight(new ABBNode(key, node));
			insertado = true;
		}else{
			insertado = insertRec(key, node->getRight());
		}
	}
	return insertado;
}

bool ABB::insert(int key){
	// TODO: implementar.
	bool insertado = false;
	if(root == nullptr){
		root = new ABBNode(key);
		insertado = true;
	}else{
		insertado = insertRec(key, root);
	}
	return insertado;
}

ABBNode* ABB::findRec(int key, ABBNode* node){
	// TODO: implementar.
	ABBNode* ans = nullptr;
	if(node != nullptr){
		if (key == node->getData()){
			// devolver nodo (donde estoy parado)
			ans = node;
		}else if(key < node->getData()){
			// entrar recursivamente a la izquierda
			ans = findRec(key, node->getLeft());
		}else{
			// entrar recursivamente a la derecha
			ans = findRec(key, node->getRight());
		}
	}
	return ans;
}

ABBNode* ABB::find(int key){
	// TODO: implementar.
	return findRec(key, root);
}

int ABB::inOrderRec(ABBNode* node, int* keys, int k){
	// TODO: implementar.
	// keys arreglo con los valores ordenados resultantes
	// k el valor iterativo
	if(node != nullptr){
		k = inOrderRec(node->getLeft(), keys, k);
		keys[k] = node->getData();
		k = k + 1;
		k = inOrderRec(node->getRight(), keys, k);
	}
	return k;
}

int ABB::inOrder(int* keys){
	// TODO: implementar.
	return inOrderRec(root, keys, 0);
}

// ---------------------------------------------------------------------------
// Parte 2 - El menor y el mayor
// ---------------------------------------------------------------------------

ABBNode* ABB::getLowest(ABBNode* node){
	ABBNode* p = node;
	if (p != nullptr){
		while (p->getLeft() != nullptr){
			p = p->getLeft();
		}
	}
	return p;
}

ABBNode* ABB::getHighest(ABBNode* node){
	ABBNode* p = node;
	if (p != nullptr){
		while (p->getRight() != nullptr){
			p = p->getRight();
		}
	}
	return p;
}

// ---------------------------------------------------------------------------
// Parte 3 - Eliminar
// ---------------------------------------------------------------------------

void ABB::updateForRemove(ABBNode* node){
	// TODO: implementar.
	ABBNode* parent = node->getParent();
	
	// q1: quien va a subir a usar el puesto del padre
	ABBNode* hijo = node->getLeft();
	if(hijo == nullptr){
		hijo = node->getRight();
	}

	// q2: si sube alguine, quien es el nuevo padre
	if(hijo != nullptr){
		hijo->setParent(parent);
	}

	// q3: donde estaba el nodo original para mover el nuevo hijo ahi
	if(parent == nullptr){
		root = hijo;
	}else if(node->isLeft()){
		parent->setLeft(hijo);
	}else{
		parent->setRight(hijo);
	}

	delete node;
}

void ABB::remove(ABBNode* node){
	// TODO: implementar.
	if (node != nullptr){
		if (node->getLeft() == nullptr || node->getRight() == nullptr){
			updateForRemove(node);
		}
		else{
			ABBNode* nodeAux = getLowest(node->getRight());
			int llave = node->getData();
			node->setData(nodeAux->getData());
			nodeAux->setData(llave);
			remove(nodeAux);
		}
	}
}
