#include <power/power.hpp>

namespace power {

	// Version 1: iterativa. Recorre de 1 a exp acumulando el producto.
	// T(n) = 1 + n => O(n)
	long long pow_it(int base, int exp, long long* steps) {
		long long ans = 1;
		*steps = 0;

		// TODO: recorrer con un for/while de 0 a exp, acumulando ans = ans * base
		// y contando cada iteracion en *steps.

		return ans;
	}

	// Version 2: recursiva simple. ans = base * pow(base, exp - 1).
	// T(n) = 1 + T(n-1) => O(n)
	long long pow_rec_simple(int base, int exp, long long* steps) {
		*steps = 0;

		// TODO: implementar la version recursiva simple.
		// Sugerencia: usar una funcion auxiliar (helper) que reciba el mismo
		// puntero steps y lo incremente en cada llamada recursiva, para no
		// perder el conteo acumulado entre llamadas.
		// Caso base: exp == 0 -> retorna 1 (sin incrementar steps).

	}

	// Version 3: recursiva divide y venceras. Reduce el exponente a la mitad
	// en cada llamada (ver Eq. 2.2 del libro / seccion 2.6).
	// T(n) = 1 + T(n/2) => O(log n)
	long long pow_rec_dc(int base, int exp, long long* steps) {
		*steps = 0;

		// TODO: implementar la version recursiva divide y venceras.
		//   - si exp es par:   aux = pow_rec_dc(base, exp/2); ans = aux * aux
		//   - si exp es impar: aux = pow_rec_dc(base, (exp-1)/2); ans = base * aux * aux
		// Igual que en la version anterior, usa una funcion auxiliar que
		// comparta el puntero steps a traves de la recursion.
		// Caso base: exp == 0 -> retorna 1 (sin incrementar steps).
	}

}
