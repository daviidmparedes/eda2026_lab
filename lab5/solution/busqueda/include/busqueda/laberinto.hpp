#ifndef BUSQUEDA_LABERINTO_HPP
#define BUSQUEDA_LABERINTO_HPP

#include "busqueda/llist.hpp"

/**
 * Parte 3 - El laberinto (libro, Cap. 5, ejercicios 2 y 3)
 *
 * Un laberinto de a lo mas 6 x 6 celdas. Cada celda esta libre ('.') o es
 * muro ('#'). Hay que llegar de una celda a otra moviendose de a un paso
 * hacia arriba, abajo, la derecha o la izquierda, sin pasar por muros.
 *
 * Las 4 preguntas del modelo:
 *     1. Estado:     (fila, col)                  id = fila * cols + col
 *     2. Inicial:    (f0, c0)                     meta: (f1, c1)
 *     3. Sucesores:  los vecinos N, S, E, O que esten libres, en ese orden
 *     4. Tamano:     filas * cols <= 36 ids  ->  bool visitado[filas * cols]
 */

const int LAB_MAX = 6;

// Cuantos sucesores puede tener una celda como maximo.
const int MAX_SUC_CELDA = 4;

/**
 * El tablero. YA VIENE RESUELTO: no hace falta leer como esta hecho, solo
 * usarlo.
 *
 * Se construye a partir de un dibujo, un string por fila:
 *
 *     const char* dibujo[] = {"..#",
 *                             "#..",
 *                             "..."};
 *     Laberinto lab(3, 3, dibujo);
 */
class Laberinto{
	private:
		int filas;
		int cols;
		char celdas[LAB_MAX][LAB_MAX];
	public:
		Laberinto(int filas_, int cols_, const char* dibujo[]);
		int getFilas();
		int getCols();
		// true si (fila, col) esta dentro del tablero y no es muro.
		bool esLibre(int fila, int col);
		// Dibuja el tablero. Si se entrega un camino, lo marca con '*', con
		// 'I' en la celda inicial y 'M' en la meta.
		void print(LList* camino = nullptr);
};

// 3a - El modelo.
// Convierte (fila, col) en su id, y de vuelta. cols es el ancho del tablero.
int idCelda(int fila, int col, int cols);
int filaDe(int id, int cols);
int colDe(int id, int cols);

// Escribe en suc los ids de las celdas vecinas LIBRES de la celda id, en el
// orden N (arriba), S (abajo), E (derecha), O (izquierda). Retorna cuantas
// escribio. suc tiene espacio para MAX_SUC_CELDA enteros.
int sucesoresCelda(Laberinto &lab, int id, int* suc);

// 3b - Busca, con una pila, un camino de (f0, c0) a (f1, c1). Retorna la lista
// de ids de celdas en orden, o nullptr si no hay camino o si alguna de las dos
// celdas no esta libre. Quien llama queda a cargo de liberar la lista.
LList* resolverLaberinto(Laberinto &lab, int f0, int c0, int f1, int c1);

// 3c - Lo mismo con una cola.
LList* resolverLaberintoCola(Laberinto &lab, int f0, int c0, int f1, int c1);

#endif
