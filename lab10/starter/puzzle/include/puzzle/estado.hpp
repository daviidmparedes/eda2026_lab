#ifndef PUZZLE_ESTADO_HPP
#define PUZZLE_ESTADO_HPP

#include <string>

/**
 * Laboratorio 10 - Un estado del puzzle 3x3 (Control 3).
 *
 * El tablero es un string de 9 caracteres, leido por filas, con '0' en el
 * hueco:
 *
 *    2 1 3
 *    0 4 5     ->   "213045786"
 *    7 8 6
 *
 * Las posiciones del string son:
 *
 *    0 1 2
 *    3 4 5
 *    6 7 8
 *
 * Las acciones son 'N', 'S', 'E' y 'O', y nombran el lado del hueco desde el
 * que entra una ficha, como en el dibujo del enunciado:
 *
 *               N
 *               |
 *               v
 *        O -> hueco <- E
 *               ^
 *               |
 *               S
 *
 * Por ejemplo, 'N' mueve la ficha que esta al norte del hueco: la ficha baja
 * y el hueco sube.
 *
 * Cada estado recuerda de cual viene (padre) y con que accion llego.
 */

// La meta.
const std::string META = "123456780";

// Las cuatro acciones.
const char ACCIONES[4] = {'N', 'S', 'E', 'O'};

class Estado{
	private:
		std::string tablero;
		Estado* padre;
		char accion;
	public:
		// ---------------------------------------------------------------
		// Dado (src/estado_dado.cpp)
		// ---------------------------------------------------------------
		// Un estado inicial no tiene padre ni accion.
		Estado(const std::string& tablero_, Estado* padre_ = nullptr, char accion_ = ' ');
		const std::string& getTablero();

		// ---------------------------------------------------------------
		// Lo que implementas (src/estado.cpp)
		// ---------------------------------------------------------------

		// true si el tablero es la META.
		bool esMeta();

		// Cuantas fichas estan en el mismo lugar que en la META. El hueco no
		// es una ficha, asi que no se cuenta. Por ejemplo, "213045786" tiene
		// prioridad 3 (el 3, el 7 y el 8) y la META tiene prioridad 8.
		int prioridad();

		// Un estado nuevo (con new) con el tablero que resulta de aplicar la
		// accion, este estado como padre y la accion con que llego. Retorna
		// nullptr si no hay ficha de ese lado del hueco. Este estado no
		// cambia.
		Estado* mover(char accion_);

		// Las acciones desde el estado inicial hasta este, en el orden en que
		// se aplican: la primera es la que se aplica al estado inicial. Un
		// estado inicial retorna "".
		std::string camino();
};

#endif
