#include "busqueda/laberinto.hpp"
#include "busqueda/stack.hpp"
#include "busqueda/queue.hpp"
#include <iostream>

// ===========================================================================
// 3a - El modelo
// ===========================================================================

// Las celdas se numeran fila por fila: en un tablero de 6 columnas, la fila 0
// tiene los ids 0..5, la fila 1 los ids 6..11, y asi.
int idCelda(int fila, int col, int cols){
	return fila * cols + col;
}

int filaDe(int id, int cols){
	return id / cols;
}

int colDe(int id, int cols){
	return id % cols;
}

// Los cuatro vecinos se recorren con dos arreglos de desplazamientos, en el
// orden N, S, E, O. esLibre ya descarta los que caen fuera del tablero.
int sucesoresCelda(Laberinto &lab, int id, int* suc){
	int cols = lab.getCols();
	int f = filaDe(id, cols);
	int c = colDe(id, cols);
	const int df[4] = {-1, 1, 0, 0};
	const int dc[4] = {0, 0, 1, -1};
	int k = 0;
	for (int d = 0; d < 4; d++){
		if (lab.esLibre(f + df[d], c + dc[d])){
			suc[k] = idCelda(f + df[d], c + dc[d], cols);
			k = k + 1;
		}
	}
	return k;
}

// ===========================================================================
// 3b - Resolver con una pila
// ===========================================================================
//
// Otra vez buscarCamino de la Parte 1. Cambian el origen, la meta y la linea
// de los sucesores.
// ---------------------------------------------------------------------------
LList* resolverLaberinto(Laberinto &lab, int f0, int c0, int f1, int c1){
	if (!lab.esLibre(f0, c0) || !lab.esLibre(f1, c1)){
		return nullptr;
	}
	int cols = lab.getCols();
	int n = lab.getFilas() * cols;
	int destino = idCelda(f1, c1, cols);

	bool visitado[LAB_MAX * LAB_MAX];
	for (int i = 0; i < n; i++){
		visitado[i] = false;
	}

	int suc[MAX_SUC_CELDA];
	Stack S;
	S.push(new DataNode(idCelda(f0, c0, cols)));
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
			int k = sucesoresCelda(lab, e->getId(), suc);
			for (int i = 0; i < k; i++){
				if (!visitado[suc[i]]){
					S.push(new DataNode(suc[i], e));
				}
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
	return camino;
}

// ===========================================================================
// 3c - Resolver con una cola
// ===========================================================================
//
// Copia exacta de resolverLaberinto. Lo unico distinto es la linea marcada.
// ---------------------------------------------------------------------------
LList* resolverLaberintoCola(Laberinto &lab, int f0, int c0, int f1, int c1){
	if (!lab.esLibre(f0, c0) || !lab.esLibre(f1, c1)){
		return nullptr;
	}
	int cols = lab.getCols();
	int n = lab.getFilas() * cols;
	int destino = idCelda(f1, c1, cols);

	bool visitado[LAB_MAX * LAB_MAX];
	for (int i = 0; i < n; i++){
		visitado[i] = false;
	}

	int suc[MAX_SUC_CELDA];
	Queue S;                                            // <- la unica diferencia
	S.push(new DataNode(idCelda(f0, c0, cols)));
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
			int k = sucesoresCelda(lab, e->getId(), suc);
			for (int i = 0; i < k; i++){
				if (!visitado[suc[i]]){
					S.push(new DataNode(suc[i], e));
				}
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
	return camino;
}

// ===========================================================================
// El tablero (YA RESUELTO)
// ===========================================================================

Laberinto::Laberinto(int filas_, int cols_, const char* dibujo[]): filas(filas_), cols(cols_){
	// Nunca mas grande que LAB_MAX x LAB_MAX.
	if (filas > LAB_MAX){ filas = LAB_MAX; }
	if (cols > LAB_MAX){ cols = LAB_MAX; }
	if (filas < 0){ filas = 0; }
	if (cols < 0){ cols = 0; }
	for (int f = 0; f < filas; f++){
		// Si una fila del dibujo viene mas corta, lo que falta se toma como muro.
		bool terminada = false;
		for (int c = 0; c < cols; c++){
			if (!terminada && dibujo[f][c] == '\0'){
				terminada = true;
			}
			if (terminada || dibujo[f][c] == '#'){
				celdas[f][c] = '#';
			}
			else{
				celdas[f][c] = '.';
			}
		}
	}
}

int Laberinto::getFilas(){
	return filas;
}

int Laberinto::getCols(){
	return cols;
}

bool Laberinto::esLibre(int fila, int col){
	if (fila < 0 || fila >= filas || col < 0 || col >= cols){
		return false;
	}
	return celdas[fila][col] == '.';
}

void Laberinto::print(LList* camino){
	char dib[LAB_MAX][LAB_MAX];
	for (int f = 0; f < filas; f++){
		for (int c = 0; c < cols; c++){
			dib[f][c] = celdas[f][c];
		}
	}

	if (camino != nullptr){
		Node* p = camino->getHead();
		int ultimo = -1;
		while (p != nullptr){
			int id = p->getId();
			// Se ignora cualquier id que no sea una celda del tablero.
			if (id >= 0 && id < filas * cols){
				if (ultimo < 0){
					dib[id / cols][id % cols] = 'I';
				}
				else{
					dib[id / cols][id % cols] = '*';
				}
				ultimo = id;
			}
			p = p->getNext();
		}
		if (ultimo >= 0 && dib[ultimo / cols][ultimo % cols] != 'I'){
			dib[ultimo / cols][ultimo % cols] = 'M';
		}
	}

	std::cout << "       ";
	for (int c = 0; c < cols; c++){
		std::cout << c << " ";
	}
	std::cout << std::endl;
	for (int f = 0; f < filas; f++){
		std::cout << "    " << f << "  ";
		for (int c = 0; c < cols; c++){
			std::cout << dib[f][c] << " ";
		}
		std::cout << std::endl;
	}
}
