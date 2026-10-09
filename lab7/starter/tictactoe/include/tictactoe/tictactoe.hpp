#ifndef TICTACTOE_TICTACTOE_HPP
#define TICTACTOE_TICTACTOE_HPP

#include "tictactoe/llist.hpp"
#include <string>

/**
 * Laboratorio 7 - Tic-tac-toe (parte practica del Control 2)
 *
 * Dos jugadores, X y O, marcan por turnos una celda vacia de un tablero de
 * 3 x 3. Siempre parte X. Gana quien complete primero una linea de tres
 * (fila, columna o diagonal). La partida termina cuando alguien gana o cuando
 * el tablero se llena.
 *
 * Un tablero es un texto de 9 caracteres: las celdas 0 a 8 en orden, con '.'
 * para una celda vacia, 'X' u 'O'.
 *
 *     0 | 1 | 2                         X | . | O
 *     3 | 4 | 5      "X.O.O.X.." es     . | O | .
 *     6 | 7 | 8                         X | . | .
 */

const int CELDAS = 9;
const int MAX_SUCESORES = 9;

// El id del tablero t.
// Esta definido para todo texto de CELDAS caracteres '.', 'X' u 'O', aunque
// el tablero no pueda aparecer en una partida. Dos tableros distintos tienen
// ids distintos, y todo id esta en el rango [0, cantidadIds()).
int idTablero(const std::string &t);

// La inversa de idTablero: el tablero que representa id.
std::string tableroDe(int id);

// Cuantos ids distintos puede entregar idTablero.
int cantidadIds();

// 'X' u 'O': el jugador al que le toca jugar en el tablero id.
char turno(int id);

// true si jugador ('X' u 'O') tiene tres en linea en el tablero id.
bool gana(int id, char jugador);

// true si id es una meta: un tablero en que X tiene tres en linea.
bool esMeta(int id);

// Escribe en suc los ids de todos los tableros que se alcanzan desde id con
// una jugada del jugador de turno, y retorna cuantos escribio. Un tablero en
// que alguien ya gano, o que esta lleno, no tiene sucesores. suc tiene
// espacio para MAX_SUCESORES enteros.
int sucesores(int id, int* suc);

// Busca, con una pila, una secuencia de jugadas desde el tablero idInicio
// hasta un tablero en que esMeta es true. Retorna una lista nueva con los ids
// de los tableros en orden, desde idInicio hasta la meta (ambos incluidos), o
// nullptr si no existe. Quien llama queda a cargo de liberar la lista.
LList* resolver(int idInicio);

#endif
