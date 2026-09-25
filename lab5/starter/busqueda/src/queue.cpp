#include "busqueda/queue.hpp"

// ===========================================================================
// 1c - La cola. AQUI TRABAJAS TU (junto con camino.cpp, jarras.cpp y
// laberinto.cpp).
//
// Mira src/stack.cpp antes de empezar: pop, front e isEmpty son casi iguales.
// Lo nuevo es push, que entra por tail, y un cuidado extra en pop.
// ===========================================================================

Queue::Queue(): head(nullptr), tail(nullptr){
}

void Queue::push(DataNode* data){
	// TODO: la ficha entra por el FINAL. Crea la caja con new StackNode(data).
	//   - Si la cola esta vacia (tail == nullptr), la caja nueva es a la vez
	//     head y tail.
	//   - Si no, engancha la caja nueva despues de tail (tail->setNext) y
	//     muevela a ser la nueva tail.
}

void Queue::pop(){
	// TODO: igual que Stack::pop, sacando por head y borrando la caja.
	// Cuidado extra: si despues de sacar la cola quedo vacia, tail tambien
	// tiene que volver a nullptr. Piensa que pasaria en el siguiente push si
	// tail siguiera apuntando a la caja que acabas de borrar.
}

StackNode* Queue::front(){
	// TODO: igual que Stack::front.
	return nullptr;
}

bool Queue::isEmpty(){
	// TODO: igual que Stack::isEmpty.
	return true;
}

Queue::~Queue(){
	while (!isEmpty()){
		pop();
	}
}
