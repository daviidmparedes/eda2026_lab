#include "power/power.hpp"
#include <iostream>
#include <fstream>
#include <string>

// Carpeta donde se escribe el .csv. La define CMakeLists.txt y apunta a la
// carpeta python/ del ejercicio, para que los datos queden junto al script que
// los grafica en vez de perderse dentro de build/.
#ifndef OUTPUT_DIR
#define OUTPUT_DIR "."
#endif

static std::string outputPath(const char* fileName){
	return std::string(OUTPUT_DIR) + "/" + fileName;
}

int main(int nargs, char** vargs) {
	int base = 2;
	int exp = 10;
	long long steps = 0;

	long long r1 = power::pow_it(base, exp, &steps);
	std::cout << "pow_it         " << base << "^" << exp << " = " << r1 << "  steps=" << steps << std::endl;

	long long r2 = power::pow_rec_simple(base, exp, &steps);
	std::cout << "pow_rec_simple " << base << "^" << exp << " = " << r2 << "  steps=" << steps << std::endl;

	long long r3 = power::pow_rec_dc(base, exp, &steps);
	std::cout << "pow_rec_dc     " << base << "^" << exp << " = " << r3 << "  steps=" << steps << std::endl;

	// Verificacion: las tres versiones deben dar el MISMO resultado.
	if (r1 == r2 && r2 == r3) {
		std::cout << "Las tres versiones coinciden [ok]" << std::endl;
	} else {
		std::cout << "ERROR: las tres versiones no coinciden" << std::endl;
	}

	// Barrido empirico: para exp = 1..maxExp, registra los pasos de cada version.
	int maxExp = 30;
	std::string csvPath = outputPath("steps.csv");
	std::ofstream out(csvPath.c_str());
	out << "exp,it,rec_simple,rec_dc" << std::endl;
	for (exp = 1; exp <= maxExp; exp++) {
		long long s_it, s_simple, s_dc;
		power::pow_it(base, exp, &s_it);
		power::pow_rec_simple(base, exp, &s_simple);
		power::pow_rec_dc(base, exp, &s_dc);
		out << exp << "," << s_it << "," << s_simple << "," << s_dc << std::endl;
	}
	out.close();
	std::cout << "Barrido escrito en " << csvPath << " (exp de 1 a " << maxExp << ")" << std::endl;

	return 0;
}
