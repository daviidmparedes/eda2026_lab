#ifndef BUSQUEDA_CAMINO_HPP
#define BUSQUEDA_CAMINO_HPP

#include "busqueda/llist.hpp"

/**
 * Parte 1 - Buscar un camino en un mapa (la clase del lunes)
 *
 * El mapa tiene n lugares, numerados de 0 a n-1 (A = 0, B = 1, ...), y se
 * guarda como un ARREGLO DE LISTAS ENLAZADAS: adj[x] es la lista de los
 * lugares a los que se puede ir directo desde x.
 *
 *     LList* adj = new LList[6];
 *     adj[0].insert(1);          // de A se puede ir a B
 *     adj[0].insert(2);          // de A se puede ir a C
 *                                // adj[0] se recorre C, B (insert es al inicio)
 *
 * Las tres busquedas usan cuatro piezas:
 *     el mapa       adj[]          no cambia
 *     la frontera   pila o cola    crece y se achica
 *     visitados     bool[n]        solo crece
 *     la historia   father         cada ficha recuerda de donde vino
 *
 * Convencion de la clase: un lugar se marca como visitado cuando SALE de la
 * frontera, no cuando entra. Por eso un mismo lugar puede estar dos veces en
 * la pila.
 */

// 1a - Retorna true si se puede llegar de origen a destino. Usa una pila.
// Si traza es true, imprime la pila al comienzo de cada iteracion, para
// compararla con las trazas hechas a mano en clases.
bool existeCamino(LList* adj, int n, int origen, int destino, bool traza = false);

// 1b - Igual que existeCamino, pero entrega el camino: una lista nueva con
// los lugares en orden, desde origen hasta destino. Retorna nullptr si no hay
// camino. Quien llama queda a cargo de liberar la lista.
LList* buscarCamino(LList* adj, int n, int origen, int destino);

// 1c - Igual que buscarCamino, pero con una cola en vez de una pila.
LList* buscarCaminoCola(LList* adj, int n, int origen, int destino);

#endif
