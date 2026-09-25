#include "busqueda/laberinto.hpp"
#include "busqueda/stack.hpp"
#include "busqueda/queue.hpp"
#include <iostream>

// ===========================================================================
// AQUI TRABAJAS TU.
//
// Parte 3: primero el modelo (3a), despues la busqueda (3b y 3c).
// La clase Laberinto (al final de este archivo) ya viene resuelta: solo
// necesitas getFilas(), getCols() y esLibre(fila, col).
// ===========================================================================

// ---------------------------------------------------------------------------
// 3a - El modelo
// ---------------------------------------------------------------------------

// Las celdas se numeran fila por fila. En un tablero de 6 columnas, la fila 0
// tiene los ids 0..5, la fila 1 los ids 6..11, y asi.
// Ejemplo para revisar a mano: con 6 columnas, (2, 3) -> 2 * 6 + 3 = 15.
//
// Usa el parametro cols, no el numero 6: no todos los laberintos son de 6 x 6.
int idCelda(int fila, int col, int cols){
	// TODO: implementar.
	return 0;
}

// De vuelta: la fila y la columna de un id.
int filaDe(int id, int cols){
	// TODO: implementar.
	return 0;
}

int colDe(int id, int cols){
	// TODO: implementar.
	return 0;
}

// Los vecinos de (f, c), en este orden:
//
//     N (arriba)      (f - 1, c)
//     S (abajo)       (f + 1, c)
//     E (derecha)     (f, c + 1)
//     O (izquierda)   (f, c - 1)
//
// Para cada uno, si lab.esLibre(fila, col), escribe su id en suc[k] y suma 1
// a k. Al final retorna k. esLibre ya se encarga de los bordes: retorna false
// si la celda cae fuera del tablero, asi que no hace falta revisarlo aparte.
//
// f y c se obtienen del id con filaDe y colDe, usando lab.getCols().
int sucesoresCelda(Laberinto &lab, int id, int* suc){
	// TODO: implementar.
	return 0;
}

// ---------------------------------------------------------------------------
// 3b - Resolver con una pila
// ---------------------------------------------------------------------------
//
// Una vez mas, copia tu buscarCamino de la Parte 1 y cambia solo esto:
//
//   - primero, si (f0, c0) o (f1, c1) no estan libres, retorna nullptr.
//   - cols = lab.getCols() y n = lab.getFilas() * cols.
//   - visitado: bool visitado[LAB_MAX * LAB_MAX], y se ponen en false los
//     primeros n.
//   - el origen es idCelda(f0, c0, cols) y el destino idCelda(f1, c1, cols).
//   - la linea de los vecinos, igual que en las jarras:
//
//         int suc[MAX_SUC_CELDA];
//         ...
//         int k = sucesoresCelda(lab, e->getId(), suc);
//         for (int i = 0; i < k; i++){ ... }
//
// Un laberinto tiene ciclos (se puede ir y volver entre dos celdas), asi que
// aca visitado es indispensable: sin el, la busqueda no termina nunca.
// ---------------------------------------------------------------------------
LList* resolverLaberinto(Laberinto &lab, int f0, int c0, int f1, int c1){
	// TODO: implementar.
	return nullptr;
}

// ---------------------------------------------------------------------------
// 3c - Resolver con una cola
// ---------------------------------------------------------------------------
//
// Copia resolverLaberinto y cambia Stack por Queue. Mira los dos caminos que
// imprime ./test en el laberinto 2.
// ---------------------------------------------------------------------------
LList* resolverLaberintoCola(Laberinto &lab, int f0, int c0, int f1, int c1){
	// TODO: implementar.
	return nullptr;
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
