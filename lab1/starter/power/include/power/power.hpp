#ifndef POWER_HPP
#define POWER_HPP

namespace power {

	// ans = base^exp. *steps recibe el numero de iteraciones/llamadas recursivas realizadas.
	long long pow_it(int base, int exp, long long* steps);
	long long pow_rec_simple(int base, int exp, long long* steps);
	long long pow_rec_dc(int base, int exp, long long* steps);

}

#endif
