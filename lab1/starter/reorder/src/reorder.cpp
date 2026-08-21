#include <reorder/reorder.hpp>

namespace reorder {

	// Reordena A in-place en O(n): negativos al inicio, no-negativos al final.
	// Usar dos punteros:
	//   i recorre desde el inicio, buscando un elemento que NO sea negativo.
	//   j recorre desde el final, buscando un elemento que NO sea positivo.
	// Cuando ambos se detienen (i encontro un no-negativo, j encontro un
	// no-positivo) y aun no se cruzan, se intercambian A[i] y A[j].
	// El algoritmo termina cuando j <= i.
	void reorder_neg_pos(int* A, int n) {
		// TODO: implementar con dos punteros i (desde 0) y j (desde n-1).
	}

}
