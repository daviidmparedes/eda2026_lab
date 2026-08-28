#ifndef POSCODE_SORT_HPP
#define POSCODE_SORT_HPP

#include <cstddef>
#include "poscode/poscode.hpp"

/**
 * Algoritmos de ordenacion para el laboratorio 3.
 *
 * Los nombres y firmas de quick_sort, merge_sort y radix_sort son los mismos
 * del repositorio oficial del curso (eda_cpp/poscodes/include/utils.hpp).
 */

// ---------------------------------------------------------------------------
// Parte 2a - ordenar n digitos (0-9) en O(n)
// ---------------------------------------------------------------------------
// Ordena un arreglo de enteros que solo toma valores en [0, 9].
void counting_sort(int *A, size_t n);

// ---------------------------------------------------------------------------
// Parte 2b - ordenar n codigos en O(n)
// ---------------------------------------------------------------------------
// Ordena los codigos mirando UN solo digito: el de la posicion d (d = 0 es el
// mas significativo). Es la pieza que usa radix_sort.
//
// Tiene que ser ESTABLE: dos codigos con el mismo digito d deben quedar en el
// mismo orden relativo que tenian antes.
void counting_sort_by_digit(Poscode *A, size_t n, size_t d);

// Ordena los n codigos en O(n) usando counting_sort_by_digit.
void radix_sort(Poscode *A, size_t n);

// ---------------------------------------------------------------------------
// Algoritmos de comparacion (ya resueltos, son los del laboratorio 2)
// ---------------------------------------------------------------------------
void quick_sort(Poscode *A, size_t n);
void merge_sort(Poscode *A, size_t n);

#endif
