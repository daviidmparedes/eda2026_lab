#include "punteros/arreglos.hpp"

int sumArray(const int *array, int n){
	// fin apunta una posicion despues del ultimo elemento. Esa direccion es
	// valida para comparar, pero no se puede desreferenciar.
	const int *fin = array + n;
	int suma = 0;
	for (const int *p = array; p < fin; p++){
		suma = suma + *p;
	}
	return suma;
}

void reverseArray(int *array, int n){
	if (n <= 1){
		return;
	}
	int *i = array;
	int *j = array + n - 1;
	while (i < j){
		int aux = *i;
		*i = *j;
		*j = aux;
		i++;
		j--;
	}
}
