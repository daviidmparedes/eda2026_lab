#include <power/power.hpp>

namespace power {

	long long pow_it(int base, int exp, long long* steps) {
		long long ans = 1;
		*steps = 0;
		for (int it = 0; it < exp; it++) {
			ans = ans * base;
			(*steps)++;
		}
		return ans;
	}

	static long long pow_rec_simple_helper(int base, int exp, long long* steps) {
		if (exp == 0) {
			return 1;
		}
		(*steps)++;
		return base * pow_rec_simple_helper(base, exp - 1, steps);
	}

	long long pow_rec_simple(int base, int exp, long long* steps) {
		*steps = 0;
		return pow_rec_simple_helper(base, exp, steps);
	}

	static long long pow_rec_dc_helper(int base, int exp, long long* steps) {
		if (exp == 0) {
			return 1;
		}
		(*steps)++;
		if (exp % 2 == 0) {
			long long aux = pow_rec_dc_helper(base, exp / 2, steps);
			return aux * aux;
		} else {
			long long aux = pow_rec_dc_helper(base, (exp - 1) / 2, steps);
			return base * aux * aux;
		}
	}

	long long pow_rec_dc(int base, int exp, long long* steps) {
		*steps = 0;
		return pow_rec_dc_helper(base, exp, steps);
	}

}
