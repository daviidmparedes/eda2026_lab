#ifndef PUNTEROS_ALCANCE_HPP
#define PUNTEROS_ALCANCE_HPP

/**
 * Tema 6 - Alcance y tiempo de vida (libro, Cap. 3.4, ejercicios 2, 5 y 10)
 *
 * Las variables locales viven en el stack y desaparecen cuando la funcion
 * termina; lo que se reserva con new vive en el heap y sigue existiendo hasta
 * que alguien lo libere. Por eso una funcion nunca puede devolver un puntero
 * a una variable local, pero si puede devolver un puntero al heap.
 */

// Entrega un arreglo de 3 elementos: {0, n / 2, n}.
// El arreglo se reserva en el heap para que siga siendo valido despues de que
// la funcion termina. Quien llama queda a cargo de liberarlo.
int* construirRango(int n);

// Entrega un arreglo nuevo con los n elementos de a seguidos de los m de b.
// Quien llama queda a cargo de liberarlo.
int* concatArray(const int *a, int n, const int *b, int m);

#endif
