#ifndef PUZZLE_VISITADOS_HPP
#define PUZZLE_VISITADOS_HPP

#include "puzzle/estado.hpp"

/**
 * Los estados que ya aparecieron en la busqueda: un ABB (laboratorio 8)
 * ordenado por el string del tablero.
 *
 * Es el dueno de los estados: los libera al destruirse. Por eso los padres de
 * cualquier estado siguen existiendo hasta el final de la busqueda.
 */
class Visitados{
	private:
		struct Nodo{
			Estado* estado;
			Nodo* left;
			Nodo* right;
		};
		Nodo* root;
		int n;
		void destruir(Nodo* nodo);
	public:
		Visitados();
		// Agrega e y retorna true. Si ese tablero ya estaba, no agrega nada y
		// retorna false: e sigue siendo de quien lo creo.
		bool agregar(Estado* e);
		// true si ese tablero ya fue agregado.
		bool contiene(const std::string& tablero);
		int size();
		~Visitados();
};

#endif
