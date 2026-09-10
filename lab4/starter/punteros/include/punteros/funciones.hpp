#ifndef PUNTEROS_FUNCIONES_HPP
#define PUNTEROS_FUNCIONES_HPP

/**
 * Tema 4 - Punteros y funciones (libro, Cap. 3.3)
 *
 * Un parametro por valor es una copia del argumento: cambiarlo no afecta a
 * quien llamo. Eso vale tambien para los punteros, que son una copia de una
 * direccion. Para que una funcion entregue resultados a quien la llama, o
 * bien recibe la direccion donde escribir, o bien recibe el parametro por
 * referencia (&).
 */

// Calcula el minimo, el maximo y el promedio del arreglo, y los entrega por
// parametros de salida. Una funcion no puede devolver tres cosas con return.
// Si n no es positivo, no modifica ninguna de las tres salidas.
void statsArray(const int *array, int n, int &min, int &max, double &prom);

// Reserva un arreglo de n enteros y los inicializa en 0.
//
// El puntero se recibe por referencia (int*&). Si se recibiera por valor
// (int*), la reserva se guardaria en la copia local y quien llama seguiria
// con su puntero original sin tocar.
void crearArreglo(int *&p, int n);

#endif
