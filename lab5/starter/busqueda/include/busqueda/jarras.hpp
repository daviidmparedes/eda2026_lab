#ifndef BUSQUEDA_JARRAS_HPP
#define BUSQUEDA_JARRAS_HPP

#include "busqueda/llist.hpp"

/**
 * Parte 2 - Las jarras
 *
 * Hay dos jarras sin marcas, A de 3 litros y B de 5 litros, y agua ilimitada.
 * Se parte con las dos vacias y se quiere dejar exactamente 'meta' litros en
 * la jarra B. Las unicas operaciones permitidas son:
 *
 *     llenar A      llenar B      vaciar A      vaciar B
 *     A -> B        B -> A        (trasvasar hasta que una se vacie o la otra se llene)
 *
 * No hay mapa: los "lugares" son los estados de las jarras, y los vecinos de
 * un estado se CALCULAN en vez de leerse de una lista. Es la misma busqueda de
 * la Parte 1 con una sola linea distinta: la que recorre los sucesores.
 *
 * Las 4 preguntas del modelo:
 *     1. Estado:     (a, b) = litros en A y en B      id = a * (CAP_B + 1) + b
 *     2. Inicial:    (0, 0)                            meta: b == meta
 *     3. Sucesores:  las 6 operaciones, en el orden de arriba, sin las que no
 *                    cambian nada
 *     4. Tamano:     (CAP_A + 1) * (CAP_B + 1) = 24 ids  ->  bool visitado[24]
 */

const int CAP_A = 3;
const int CAP_B = 5;
const int N_JARRAS = (CAP_A + 1) * (CAP_B + 1);

// Cuantos sucesores puede tener un estado como maximo (una por operacion).
const int MAX_SUC_JARRAS = 6;

// 2a - El modelo.
// Convierte el estado (a, b) en su id, y de vuelta.
int idJarras(int a, int b);
int litrosA(int id);
int litrosB(int id);

// Escribe en suc los ids de los estados que se alcanzan desde id con UNA
// operacion, en el orden: llenar A, llenar B, vaciar A, vaciar B, A -> B,
// B -> A. Las operaciones que dejan el estado igual no se escriben.
// Retorna cuantos escribio. suc tiene espacio para MAX_SUC_JARRAS enteros.
int sucesoresJarras(int id, int* suc);

// 2b - Busca, con una pila, una secuencia de estados desde (0, 0) hasta un
// estado con 'meta' litros en B. Retorna la lista de ids en orden, o nullptr
// si no se puede. Quien llama queda a cargo de liberar la lista.
LList* resolverJarras(int meta);

// 2c - Lo mismo con una cola.
LList* resolverJarrasCola(int meta);

#endif
