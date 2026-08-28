#ifndef POSCODE_UTILS_HPP
#define POSCODE_UTILS_HPP

#include <string>
#include "poscode/poscode.hpp"

/**
 * Utilidades del laboratorio: lectura del dataset, copias y verificaciones.
 *
 * TODO ESTO YA ESTA RESUELTO. No necesitas leer archivos ni parsear nada para
 * hacer el laboratorio: tu trabajo es solo ordenar el arreglo de Poscode que
 * readCodes() te entrega.
 */

// Cuantos codigos hay guardados en el archivo (lee solo la cabecera).
// Retorna 0 si el archivo no existe o no tiene el formato esperado.
size_t countCodes(const std::string &strfile);

// Lee los primeros n codigos del archivo binario y los deja en un arreglo
// recien reservado de tamano n. Retorna nullptr si algo falla (archivo
// inexistente, formato invalido, o n mayor que la cantidad disponible).
// Mismo nombre y firma que eda_cpp/poscodes/include/utils.hpp.
Poscode *readCodes(const std::string &strfile, size_t n);

// Libera un arreglo entregado por readCodes() o copyCodes().
void deleteCodes(Poscode *codes);

// Copia independiente de A. Cada algoritmo se mide sobre los MISMOS datos,
// asi que siempre se ordena una copia y nunca el arreglo original.
Poscode *copyCodes(Poscode *A, size_t n);

// true si A quedo ordenado de menor a mayor.
bool isSorted(Poscode *A, size_t n);

// Imprime los codigos en una linea (para arreglos chicos).
void printCodes(Poscode *A, size_t n);

// Numero entero al azar en [min, max]. Identica a la de eda_cpp/sort/src/utils.cpp;
// la ocupa quick_sort para elegir el pivote.
int getRandomInt(int min, int max);

#endif
