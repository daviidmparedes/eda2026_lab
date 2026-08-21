#include "power/power.hpp"
#include <iostream>
#include <fstream>

int main(int nargs, char** vargs) {
	int base = 2;
	int exp = 15;
	long long steps = 0;

	long long r1 = power::pow_it(base, exp, &steps);
	std::cout << "pow_it       " << base << "^" << exp << " = " << r1 << "  steps=" << steps << std::endl;

	long long r2 = power::pow_rec_simple(base, exp, &steps);
	std::cout << "pow_rec_simple " << base << "^" << exp << " = " << r2 << "  steps=" << steps << std::endl;

	long long r3 = power::pow_rec_dc(base, exp, &steps);
	std::cout << "pow_rec_dc   " << base << "^" << exp << " = " << r3 << "  steps=" << steps << std::endl;

	// Barrido empirico: para exp = 1..maxExp, registra los pasos de cada version en steps.csv
	int maxExp = 40;
	std::ofstream out("steps.csv");
	out << "exp,it,rec_simple,rec_dc" << std::endl;
	for (exp = 1; exp <= maxExp; exp++) {
		long long s_it, s_simple, s_dc;
		power::pow_it(base, exp, &s_it);
		power::pow_rec_simple(base, exp, &s_simple);
		power::pow_rec_dc(base, exp, &s_dc);
		out << exp << "," << s_it << "," << s_simple << "," << s_dc << std::endl;
	}
	out.close();
	std::cout << "Barrido escrito en steps.csv (exp de 1 a " << maxExp << ")" << std::endl;

	return 0;
}
