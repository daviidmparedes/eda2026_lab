#ifndef POSCODE_POSCODE_HPP
#define POSCODE_POSCODE_HPP

#include <string>

/**
 * Un codigo de la Prueba 1: una secuencia de digitos (0-9) de largo fijo.
 *
 * Es la misma clase que usa el repositorio oficial del curso
 * (eda_cpp/poscodes/include/poscode.hpp). Ya viene implementada: para el
 * laboratorio solo necesitas dos cosas de ella.
 *
 *   getValue(i)  ->  el caracter que esta en la posicion i, contando desde la
 *                    izquierda (i = 0 es el digito MAS significativo).
 *                    Ojo: devuelve un caracter, no un numero. Para pasar de
 *                    '7' al entero 7 hay que restarle '0':
 *
 *                        int digito = codigo.getValue(i) - '0';
 *
 *   getData()    ->  el string completo, util para comparar dos codigos.
 *                    Como todos tienen el mismo largo, el orden alfabetico
 *                    de los strings coincide con el orden numerico:
 *                    "05893" < "13802" < "83471".
 */
class Poscode{
	private:
		std::string data;
	public:
		Poscode();
		Poscode(std::string _data);
		char getValue(size_t i);
		const std::string &getData();
};

#endif
