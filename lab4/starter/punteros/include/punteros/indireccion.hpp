#ifndef PUNTEROS_INDIRECCION_HPP
#define PUNTEROS_INDIRECCION_HPP

/**
 * Tema 1 - Punteros e indireccion (libro, Cap. 3.1)
 *
 * El operador * desreferencia: entrega el contenido que hay en la direccion
 * guardada por un puntero. El operador & referencia: entrega la direccion de
 * una variable, incluso si esa variable ya es un puntero.
 */

// Entrega el entero que esta al final de tres niveles de indireccion.
// ppp apunta a un puntero, que apunta a otro puntero, que apunta al entero.
int valorFinal(int ***ppp);

// Hace que el puntero apuntado por pp pase a apuntar a destino, y despues
// escribe valor en ese destino.
//
// Son dos escrituras en niveles distintos: una cambia a que apunta el puntero,
// la otra cambia el entero apuntado.
void reapuntar(int **pp, int *destino, int valor);

#endif
