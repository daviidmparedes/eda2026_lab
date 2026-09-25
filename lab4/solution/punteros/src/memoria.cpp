#include "punteros/memoria.hpp"

int* reservarLleno(int n, int valor){
	if (n <= 0){
		return nullptr;
	}
	int *a = new int[n];
	for (int i = 0; i < n; i++){
		a[i] = valor;
	}
	return a;
}

void liberarYAnular(int *&p){
	// delete[] sobre nullptr es una operacion valida y no hace nada, asi que
	// no hace falta preguntar si p es nulo antes de liberar.
	delete[] p;
	p = nullptr;
}
