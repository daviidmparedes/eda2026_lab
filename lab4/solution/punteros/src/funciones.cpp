#include "punteros/funciones.hpp"

void statsArray(const int *array, int n, int &min, int &max, double &prom){
	if (n <= 0){
		return;
	}
	// min, max y prom son alias de las variables de quien llama: escribir en
	// ellos escribe alla.
	min = array[0];
	max = array[0];
	long suma = array[0];
	for (int i = 1; i < n; i++){
		if (array[i] < min){ min = array[i]; }
		if (array[i] > max){ max = array[i]; }
		suma = suma + array[i];
	}
	// El cast es necesario: sin el, la division seria entera y el promedio
	// perderia los decimales.
	prom = static_cast<double>(suma) / n;
}

void crearArreglo(int *&p, int n){
	if (n <= 0){
		p = nullptr;
		return;
	}
	p = new int[n];
	for (int i = 0; i < n; i++){
		p[i] = 0;
	}
}
