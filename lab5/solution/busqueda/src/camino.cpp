#include "busqueda/camino.hpp"
#include "busqueda/stack.hpp"
#include "busqueda/queue.hpp"
#include <iostream>

// ===========================================================================
// Ayuda para la traza (ya resuelta)
// ===========================================================================

// Nombre de un lugar del mapa: 0 -> A, 1 -> B, ...
static char nombre(int id){
	return static_cast<char>('A' + id);
}

// Imprime las cajas desde el fondo hasta el tope, que es como se dibuja la
// pila en la pizarra. p apunta al tope; la recursion llega primero al fondo y
// va imprimiendo a la vuelta.
static void imprimirDesdeFondo(StackNode* p){
	if (p != nullptr){
		imprimirDesdeFondo(p->getNext());
		std::cout << nombre(p->getData()->getId()) << " ";
	}
}

// Una fila de la tabla de traza: numero de iteracion, la pila y su tope.
// Llamala al comienzo de cada iteracion de existeCamino, cuando traza es true.
void imprimirIteracion(int it, Stack &S){
	std::cout << "    it " << it << " | pila (fondo -> tope): ";
	imprimirDesdeFondo(S.front());
	std::cout << "| front: " << nombre(S.front()->getData()->getId()) << std::endl;
}

// ===========================================================================
// 1a - ¿Existe un camino?
// ===========================================================================
//
// Es el pseudocodigo v1 de la clase, linea por linea:
//
//      visitado[0..n-1] <- falso
//      S <- pila vacia;  S.push(origen);  encontrado <- falso
//      mientras !S.isEmpty() y !encontrado
//          e <- S.front()
//          si e = destino:  encontrado <- verdadero
//          si no:           visitado[e] <- verdadero;  S.pop()
//                           para cada x en adj[e]:  si !visitado[x]: S.push(x)
//      retornar encontrado
//
// Aca no hace falta la historia, asi que basta con guardar el id del tope
// ANTES del pop: pop libera la ficha (no tiene hijas) y despues no se puede
// volver a leer.
// ---------------------------------------------------------------------------
bool existeCamino(LList* adj, int n, int origen, int destino, bool traza){
	bool* visitado = new bool[n];
	for (int i = 0; i < n; i++){
		visitado[i] = false;
	}

	Stack S;
	S.push(new DataNode(origen));
	bool encontrado = false;
	int it = 0;
	while (!S.isEmpty() && !encontrado){
		it = it + 1;
		if (traza){
			imprimirIteracion(it, S);
		}
		int e = S.front()->getData()->getId();
		if (e == destino){
			encontrado = true;
		}
		else{
			visitado[e] = true;
			S.pop();
			Node* x = adj[e].getHead();
			while (x != nullptr){
				if (!visitado[x->getId()]){
					S.push(new DataNode(x->getId()));
				}
				x = x->getNext();
			}
		}
	}
	if (traza && !encontrado){
		std::cout << "    pila vacia: no hay camino" << std::endl;
	}

	delete[] visitado;
	return encontrado;
}

// ===========================================================================
// 1b - ¿Cual es el camino?
// ===========================================================================
//
// El pseudocodigo v2: la misma busqueda, con dos lineas distintas y un bloque
// nuevo al final.
//
//   - Cada ficha nueva se anota con su padre:  new DataNode(x, e).
//   - Al encontrar la meta, se siguen los father desde ella hasta el origen.
//     Salen al reves, pero como LList::insert inserta al inicio, la lista
//     queda en orden sin paso extra.
//
// El detalle de memoria: aca e SI se usa despues del pop (es el padre de las
// fichas nuevas), y pop libera la ficha si no tiene hijas, que es justo lo
// que le pasa a e en ese momento. Por eso se le suma una hija de mentira
// antes del pop y se le quita al terminar de apilar.
// ---------------------------------------------------------------------------
LList* buscarCamino(LList* adj, int n, int origen, int destino){
	bool* visitado = new bool[n];
	for (int i = 0; i < n; i++){
		visitado[i] = false;
	}

	Stack S;
	S.push(new DataNode(origen));
	DataNode* e = nullptr;
	bool encontrado = false;
	while (!S.isEmpty() && !encontrado){
		e = S.front()->getData();
		if (e->getId() == destino){
			encontrado = true;
		}
		else{
			visitado[e->getId()] = true;
			e->setNChildren(e->getNChildren() + 1);     // retener: el pop no la libera
			S.pop();
			Node* x = adj[e->getId()].getHead();
			while (x != nullptr){
				if (!visitado[x->getId()]){
					S.push(new DataNode(x->getId(), e));
				}
				x = x->getNext();
			}
			e->setNChildren(e->getNChildren() - 1);     // soltar
			if (e->getNChildren() == 0){
				// No tuvo hijas: nadie la va a necesitar como padre.
				delete e;
			}
		}
	}

	LList* camino = nullptr;
	if (encontrado){
		camino = new LList();
		DataNode* p = e;
		while (p != nullptr){
			camino->insert(p->getId());
			p = p->getFather();
		}
	}

	delete[] visitado;
	// Lo que quedo en la pila (incluida la ficha de la meta y, en cascada, sus
	// antecesores) se libera solo al destruirse S.
	return camino;
}

// ===========================================================================
// 1c - El camino, con una cola
// ===========================================================================
//
// Copia exacta de buscarCamino. Lo unico distinto es la linea marcada.
// ---------------------------------------------------------------------------
LList* buscarCaminoCola(LList* adj, int n, int origen, int destino){
	bool* visitado = new bool[n];
	for (int i = 0; i < n; i++){
		visitado[i] = false;
	}

	Queue S;                                            // <- la unica diferencia
	S.push(new DataNode(origen));
	DataNode* e = nullptr;
	bool encontrado = false;
	while (!S.isEmpty() && !encontrado){
		e = S.front()->getData();
		if (e->getId() == destino){
			encontrado = true;
		}
		else{
			visitado[e->getId()] = true;
			e->setNChildren(e->getNChildren() + 1);
			S.pop();
			Node* x = adj[e->getId()].getHead();
			while (x != nullptr){
				if (!visitado[x->getId()]){
					S.push(new DataNode(x->getId(), e));
				}
				x = x->getNext();
			}
			e->setNChildren(e->getNChildren() - 1);
			if (e->getNChildren() == 0){
				delete e;
			}
		}
	}

	LList* camino = nullptr;
	if (encontrado){
		camino = new LList();
		DataNode* p = e;
		while (p != nullptr){
			camino->insert(p->getId());
			p = p->getFather();
		}
	}

	delete[] visitado;
	return camino;
}
