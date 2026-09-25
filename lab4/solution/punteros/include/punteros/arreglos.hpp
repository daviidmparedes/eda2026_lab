#ifndef PUNTEROS_ARREGLOS_HPP
#define PUNTEROS_ARREGLOS_HPP

/**
 * Tema 3 - Punteros y arreglos (libro, Cap. 3.2 y 3.2.1)
 *
 * Un arreglo se representa con un puntero a su primer elemento, y por eso
 * array[i] y *(array + i) son lo mismo. Sumarle 1 a un puntero lo mueve al
 * siguiente elemento, no al siguiente byte: el compilador sabe cuanto ocupa
 * el tipo apuntado.
 */

// Suma los n elementos del arreglo.
// Se recorre con aritmetica de punteros, sin usar el operador [].
int sumArray(const int *array, int n);

// Invierte el arreglo in-place.
// Se usan dos punteros que avanzan uno hacia el otro, sin variables de indice.
void reverseArray(int *array, int n);

#endif
