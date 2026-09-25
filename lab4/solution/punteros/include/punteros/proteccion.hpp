#ifndef PUNTEROS_PROTECCION_HPP
#define PUNTEROS_PROTECCION_HPP

/**
 * Tema 5 - Proteger los datos de entrada
 *
 * Cuando una funcion recibe un arreglo, recibe un puntero, y con ese puntero
 * puede modificar el original. Para impedirlo se declara const el contenido
 * apuntado:
 *
 *     const int *array;   protege el contenido; el puntero si se puede mover
 *     int *const array;   protege el puntero; el contenido si se puede cambiar
 *
 * Los parametros de solo lectura de este laboratorio usan la primera forma.
 */

// Cuenta cuantos elementos del arreglo son menores que limite.
// array es const: la funcion lee el arreglo y no puede modificarlo.
int contarMenores(const int *array, int n, int limite);

// Copia los n elementos de origen en destino.
// Solo origen es const: destino es justamente lo que hay que escribir.
void copiarArreglo(const int *origen, int *destino, int n);

#endif
