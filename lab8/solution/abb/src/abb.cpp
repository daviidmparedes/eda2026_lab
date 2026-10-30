#include "abb/abb.hpp"

// ===========================================================================
// SOLUCION DE REFERENCIA
// ===========================================================================

// ---------------------------------------------------------------------------
// Parte 1 - Insertar, buscar y recorrer
// ---------------------------------------------------------------------------

// Libro, Alg. 19, con dos cambios: una llave repetida no se inserta, y el
// nodo nuevo se crea con su padre.
bool ABB::insertRec(int key, ABBNode* node){
	bool insertado = false;
	if (key < node->getData()){
		if (node->getLeft() == nullptr){
			node->setLeft(new ABBNode(key, node));
			insertado = true;
		}
		else{
			insertado = insertRec(key, node->getLeft());
		}
	}
	else if (key > node->getData()){
		if (node->getRight() == nullptr){
			node->setRight(new ABBNode(key, node));
			insertado = true;
		}
		else{
			insertado = insertRec(key, node->getRight());
		}
	}
	return insertado;
}

bool ABB::insert(int key){
	bool insertado = false;
	if (root == nullptr){
		root = new ABBNode(key);
		insertado = true;
	}
	else{
		insertado = insertRec(key, root);
	}
	return insertado;
}

// Libro, Alg. 20, revisando primero si node es nullptr: una llave que no
// esta termina bajando a un hijo que no existe, y ahi la respuesta es nullptr.
ABBNode* ABB::findRec(int key, ABBNode* node){
	ABBNode* ans = nullptr;
	if (node != nullptr){
		if (key == node->getData()){
			ans = node;
		}
		else if (key < node->getData()){
			ans = findRec(key, node->getLeft());
		}
		else{
			ans = findRec(key, node->getRight());
		}
	}
	return ans;
}

ABBNode* ABB::find(int key){
	return findRec(key, root);
}

// In-orden: el subarbol izquierdo, el nodo y el subarbol derecho. En un ABB
// entrega las llaves de menor a mayor. k es la proxima posicion libre de keys.
int ABB::inOrderRec(ABBNode* node, int* keys, int k){
	if (node != nullptr){
		k = inOrderRec(node->getLeft(), keys, k);
		keys[k] = node->getData();
		k = k + 1;
		k = inOrderRec(node->getRight(), keys, k);
	}
	return k;
}

int ABB::inOrder(int* keys){
	return inOrderRec(root, keys, 0);
}

// ---------------------------------------------------------------------------
// Parte 2 - El menor y el mayor
// ---------------------------------------------------------------------------

// Las llaves menores siempre estan a la izquierda: el menor es el ultimo nodo
// al bajar por la izquierda.
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
// Parte 3 - Eliminar (el algoritmo de la clase)
// ---------------------------------------------------------------------------

// update_for_remove de la clase. Los tres casos del dibujo (hoja, solo hijo
// derecho, solo hijo izquierdo) son el mismo movimiento: el unico hijo de
// node, o nullptr si es una hoja, pasa a ocupar su lugar. El caso que el
// dibujo no tiene es la raiz: sin padre, el que cambia es root.
void ABB::updateForRemove(ABBNode* node){
	ABBNode* parent = node->getParent();
	ABBNode* hijo = node->getLeft();
	if (hijo == nullptr){
		hijo = node->getRight();
	}
	if (hijo != nullptr){
		hijo->setParent(parent);
	}
	if (parent == nullptr){
		root = hijo;
	}
	else if (node->isLeft()){
		parent->setLeft(hijo);
	}
	else{
		parent->setRight(hijo);
	}
	delete node;
}

// ABB_remove de la clase. Con dos hijos se busca el menor de los mayores (el
// menor del subarbol derecho) y se intercambian las llaves. La llave que hay
// que sacar queda en nodeAux, que no tiene hijo izquierdo, asi que la llamada
// recursiva cae en updateForRemove. Con el mayor de los menores
// (getHighest(node->getLeft())) funciona igual.
void ABB::remove(ABBNode* node){
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
