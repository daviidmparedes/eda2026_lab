#include "punteros/indireccion.hpp"

int valorFinal(int ***ppp){
	// Un * por cada nivel declarado: ppp -> **ppp -> *ppp -> entero.
	return ***ppp;
}

void reapuntar(int **pp, int *destino, int valor){
	// *pp es el puntero que hay que mover.
	*pp = destino;
	// **pp es el entero al que ahora apunta.
	**pp = valor;
}
