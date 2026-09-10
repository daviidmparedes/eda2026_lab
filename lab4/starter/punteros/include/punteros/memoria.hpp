#ifndef PUNTEROS_MEMORIA_HPP
#define PUNTEROS_MEMORIA_HPP

/**
 * Tema 2 - Memoria dinamica (libro, Cap. 3.2)
 *
 * new[] reserva memoria en el heap y delete[] la devuelve. Toda reserva tiene
 * que tener su liberacion, y la forma de liberar debe coincidir con la de
 * reservar: lo que se pide con new[] se libera con delete[].
 */

// Reserva un arreglo de n enteros en el heap y lo llena con valor.
// Entrega nullptr si n no es positivo.
// Quien llama queda a cargo de liberarlo con liberarYAnular.
int* reservarLleno(int n, int valor);

// Libera el arreglo apuntado por p y deja p en nullptr.
//
// p se recibe por referencia (int*&) y no por valor: hay que modificar el
// puntero de quien llama, no una copia. Dejarlo en nullptr evita que quede
// apuntando a memoria ya devuelta.
void liberarYAnular(int *&p);

#endif
