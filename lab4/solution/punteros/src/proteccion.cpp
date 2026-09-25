#include "punteros/proteccion.hpp"

int contarMenores(const int *array, int n, int limite){
	int cuenta = 0;
	for (int i = 0; i < n; i++){
		if (array[i] < limite){
			cuenta = cuenta + 1;
		}
	}
	return cuenta;
	// Agregar aca array[0] = 0; no compilaria: array apunta a enteros const.
}

void copiarArreglo(const int *origen, int *destino, int n){
	for (int i = 0; i < n; i++){
		destino[i] = origen[i];
	}
}
