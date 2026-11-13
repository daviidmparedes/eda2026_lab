#include "puzzle/visitados.hpp"

Visitados::Visitados(): root(nullptr), n(0){
}

// Baja comparando tableros, como el insert del ABB, hasta encontrar el tablero
// o el lugar donde tiene que ir.
bool Visitados::agregar(Estado* e){
	Nodo* nuevo = new Nodo{e, nullptr, nullptr};
	if (root == nullptr){
		root = nuevo;
		n = 1;
		return true;
	}
	Nodo* p = root;
	while (p->estado->getTablero() != e->getTablero()){
		if (e->getTablero() < p->estado->getTablero()){
			if (p->left == nullptr){
				p->left = nuevo;
				n = n + 1;
				return true;
			}
			p = p->left;
		}
		else{
			if (p->right == nullptr){
				p->right = nuevo;
				n = n + 1;
				return true;
			}
			p = p->right;
		}
	}
	delete nuevo;
	return false;
}

bool Visitados::contiene(const std::string& tablero){
	Nodo* p = root;
	while (p != nullptr && p->estado->getTablero() != tablero){
		if (tablero < p->estado->getTablero()){
			p = p->left;
		}
		else{
			p = p->right;
		}
	}
	return p != nullptr;
}

int Visitados::size(){
	return n;
}

void Visitados::destruir(Nodo* nodo){
	if (nodo != nullptr){
		destruir(nodo->left);
		destruir(nodo->right);
		delete nodo->estado;
		delete nodo;
	}
}

Visitados::~Visitados(){
	destruir(root);
}
