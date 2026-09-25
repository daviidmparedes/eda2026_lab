#include "punteros/alcance.hpp"

int* construirRango(int n){
	// El arreglo va en el heap. Declararlo como int v[3] local y devolver v
	// entregaria un puntero a memoria que deja de existir al salir de aca.
	int *v = new int[3];
	v[0] = 0;
	v[1] = n / 2;
	v[2] = n;
	return v;
}

int* concatArray(const int *a, int n, const int *b, int m){
	int total = n + m;
	if (total <= 0){
		return nullptr;
	}
	int *r = new int[total];
	for (int i = 0; i < n; i++){
		r[i] = a[i];
	}
	for (int i = 0; i < m; i++){
		// Los elementos de b empiezan donde terminaron los de a.
		r[n + i] = b[i];
	}
	return r;
}
