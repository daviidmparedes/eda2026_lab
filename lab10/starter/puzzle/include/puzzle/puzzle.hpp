#ifndef PUZZLE_PUZZLE_HPP
#define PUZZLE_PUZZLE_HPP

#include <string>

/**
 * Laboratorio 10 - Resolver el puzzle 3x3 (Control 3).
 *
 * Las dos funciones reciben un tablero inicial (por ejemplo "213045786") y
 * buscan una secuencia de acciones que lo deje en la META. Para buscar usan
 * las clases dadas: Cola, MaxHeap y Visitados.
 */

struct Resultado{
	// true si se encontro una solucion; false si el tablero inicial no
	// puede llegar a la META.
	bool encontrado;
	// Las acciones, en orden, desde el tablero inicial hasta la META. Por
	// ejemplo "EES". Si no hay solucion, "".
	std::string acciones;
	// Cuantos estados salieron de la frontera durante la busqueda.
	int explorados;
};

// Pregunta 1: la frontera es una Cola. La solucion tiene que ser de largo
// minimo.
Resultado resolverConCola(const std::string& inicio);

// Pregunta 2: la frontera es un MaxHeap, y la prioridad de cada estado es la
// cantidad de fichas en su lugar (Estado::prioridad). La solucion no
// necesita ser la mas corta.
Resultado resolverConHeap(const std::string& inicio);

#endif
