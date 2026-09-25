#include "busqueda/camino.hpp"
#include "busqueda/stack.hpp"
#include "busqueda/queue.hpp"
#include <iostream>

// ===========================================================================
// AQUI TRABAJAS TU.
//
// Parte 1: tres funciones en orden, cada una parte de la anterior.
//
//   1a. existeCamino       -> el pseudocodigo v1 de la clase
//   1b. buscarCamino       -> el pseudocodigo v2: v1 + la historia
//   1c. buscarCaminoCola   -> 1b con una cola (antes implementa src/queue.cpp)
//
// Mientras una funcion este sin implementar, ./test la marca como FALLA, pero
// el resto sigue corriendo igual.
// ===========================================================================

// ---------------------------------------------------------------------------
// Ayuda para la traza (ya resuelta)
// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------
// 1a - ¿Existe un camino?
// ---------------------------------------------------------------------------
//
// El pseudocodigo de la clase:
//
//      visitado[0..n-1] <- falso
//      S <- pila vacia
//      S.push(origen)
//      encontrado <- falso
//      mientras !S.isEmpty() y !encontrado
//          e <- S.front()
//          si e = destino
//              encontrado <- verdadero
//          si no
//              visitado[e] <- verdadero
//              S.pop()
//              para cada x en adj[e]
//                  si !visitado[x]
//                      S.push(x)
//      retornar encontrado
//
// Guarda el id ANTES del pop: pop libera la ficha, y despues ya no se puede
// leer.
//
// La traza: lleva un contador de iteraciones y, si traza es true, llama a
// imprimirIteracion(it, S) al comienzo de cada vuelta del while.
//
// No olvides el delete[] de visitado antes de retornar.
// ---------------------------------------------------------------------------
bool existeCamino(LList* adj, int n, int origen, int destino, bool traza){
	// TODO: implementar.
	return false;
}

// ---------------------------------------------------------------------------
// 1b - ¿Cual es el camino?
// ---------------------------------------------------------------------------
//
// Parte copiando tu existeCamino. Cambian dos lineas y se agrega un bloque:
//
//   - e deja de ser un int: es la ficha completa.
//         DataNode* e = S.front()->getData();
//     Declarala ANTES del while: la vas a necesitar despues.
//
//   - Cada ficha nueva se anota con su padre:
//         S.push(new DataNode(x->getId(), e));
//
//   - Si se encontro la meta, se arma el camino siguiendo los father desde e
//     hasta llegar a nullptr, insertando cada id en una LList nueva. Salen al
//     reves, pero como insert() inserta al inicio, la lista queda en orden.
//     Si no se encontro, se retorna nullptr.
//
// CUIDADO CON LA MEMORIA. Aca e SI se usa despues del pop: es el padre de las
// fichas que apilas. Pero pop libera la ficha si no tiene hijas, y en ese
// momento e todavia no tiene ninguna. Si haces el pop tal cual, e queda
// apuntando a memoria liberada. La solucion es sumarle una hija "de mentira"
// mientras dura el pop:
//
//         e->setNChildren(e->getNChildren() + 1);    // retener
//         S.pop();
//         ... apilar las hijas ...
//         e->setNChildren(e->getNChildren() - 1);    // soltar
//         if (e->getNChildren() == 0){
//             delete e;                              // no tuvo hijas
//         }
//
// Lo que quede en la pila al terminar se libera solo cuando S se destruye.
// ---------------------------------------------------------------------------
LList* buscarCamino(LList* adj, int n, int origen, int destino){
	// TODO: implementar.
	return nullptr;
}

// ---------------------------------------------------------------------------
// 1c - El camino, con una cola
// ---------------------------------------------------------------------------
//
// Primero implementa la cola en src/queue.cpp. Despues copia tu buscarCamino
// completo y cambia UNA sola palabra: Stack por Queue.
// ---------------------------------------------------------------------------
LList* buscarCaminoCola(LList* adj, int n, int origen, int destino){
	// TODO: implementar.
	return nullptr;
}
