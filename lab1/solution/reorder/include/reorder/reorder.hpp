#ifndef REORDER_HPP
#define REORDER_HPP

namespace reorder {

	// Reordena A in-place en O(n): negativos al inicio, no-negativos al final.
	// El orden relativo dentro de cada grupo no se preserva.
	void reorder_neg_pos(int* A, int n);

}

#endif
