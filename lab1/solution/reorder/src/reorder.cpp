#include <reorder/reorder.hpp>

namespace reorder {

	void reorder_neg_pos(int* A, int n) {
		int i = 0;
		int j = n - 1;
		while (j > i) {
			while (i < n && A[i] < 0) {
				i++;
			}
			while (j >= 0 && A[j] >= 0) {
				j--;
			}
			if (j > i) {
				int tmp = A[i];
				A[i] = A[j];
				A[j] = tmp;
				i++;
				j--;
			}
		}
	}

}
