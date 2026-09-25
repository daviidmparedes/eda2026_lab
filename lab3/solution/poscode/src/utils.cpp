#include "poscode/utils.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

// ---------------------------------------------------------------------------
// Lectura del dataset binario (lab3/data/codes_1M.bin)
//
// Formato del archivo, todos los enteros uint32 little-endian:
//
//     offset  0   magic    4 bytes  "EDAP"
//     offset  4   version  uint32   = 1
//     offset  8   count    uint32   cantidad de codigos guardados
//     offset 12   digits   uint32   digitos por codigo (= 5)
//     offset 16   payload  count * uint32, cada valor en [0, 10^digits)
//
// Cada codigo se guarda como su valor numerico ("05893" -> 5893) y al leerlo
// se rellena con ceros a la izquierda hasta 'digits' caracteres, asi que la
// conversion no pierde informacion.
//
// El archivo lo genera lab3/data/generate_codes.py.
// ---------------------------------------------------------------------------

static const size_t HEADER_SIZE = 16;

// Arma un uint32 a partir de 4 bytes en orden little-endian. Se hace byte a
// byte a proposito: asi el archivo se lee igual en cualquier maquina, sin
// depender del endianness del procesador.
static unsigned int readU32(const unsigned char *b){
	return static_cast<unsigned int>(b[0])
	     | (static_cast<unsigned int>(b[1]) << 8)
	     | (static_cast<unsigned int>(b[2]) << 16)
	     | (static_cast<unsigned int>(b[3]) << 24);
}

// Lee la cabecera y valida el formato. Retorna false si el archivo no sirve.
static bool readHeader(std::ifstream &in, unsigned int &count, unsigned int &digits){
	unsigned char header[HEADER_SIZE];
	in.read(reinterpret_cast<char*>(header), HEADER_SIZE);
	if (in.gcount() != static_cast<std::streamsize>(HEADER_SIZE)){
		std::cerr << "Error: el archivo es demasiado corto." << std::endl;
		return false;
	}
	if (header[0] != 'E' || header[1] != 'D' || header[2] != 'A' || header[3] != 'P'){
		std::cerr << "Error: el archivo no tiene el formato esperado." << std::endl;
		return false;
	}
	unsigned int version = readU32(header + 4);
	if (version != 1){
		std::cerr << "Error: version de archivo desconocida (" << version << ")." << std::endl;
		return false;
	}
	count = readU32(header + 8);
	digits = readU32(header + 12);
	return true;
}

size_t countCodes(const std::string &strfile){
	std::ifstream in(strfile.c_str(), std::ios::binary);
	if (!in.is_open()){
		return 0;
	}
	unsigned int count = 0;
	unsigned int digits = 0;
	if (!readHeader(in, count, digits)){
		return 0;
	}
	return static_cast<size_t>(count);
}

Poscode *readCodes(const std::string &strfile, size_t n){
	std::ifstream in(strfile.c_str(), std::ios::binary);
	if (!in.is_open()){
		std::cerr << "Error: no pude abrir " << strfile << std::endl;
		std::cerr << "       genera el dataset con: python3 lab3/data/generate_codes.py" << std::endl;
		return nullptr;
	}

	unsigned int count = 0;
	unsigned int digits = 0;
	if (!readHeader(in, count, digits)){
		return nullptr;
	}
	if (n > static_cast<size_t>(count)){
		std::cerr << "Error: se pidieron " << n << " codigos y el archivo solo tiene "
		          << count << "." << std::endl;
		return nullptr;
	}

	// Se leen los n * 4 bytes de una sola vez y despues se decodifican en
	// memoria. Leer valor por valor desde el disco seria mucho mas lento.
	std::vector<unsigned char> buffer(n * 4);
	if (n > 0){
		in.read(reinterpret_cast<char*>(&buffer[0]), static_cast<std::streamsize>(n * 4));
		if (in.gcount() != static_cast<std::streamsize>(n * 4)){
			std::cerr << "Error: el archivo se corto antes de tiempo." << std::endl;
			return nullptr;
		}
	}
	in.close();

	Poscode *codes = new Poscode[n];
	std::string text(digits, '0');
	for (size_t i = 0; i < n; i++){
		unsigned int value = readU32(&buffer[i * 4]);
		// Se rellena de derecha a izquierda; lo que sobra queda en '0'.
		for (size_t d = digits; d-- > 0; ){
			text[d] = static_cast<char>('0' + (value % 10));
			value = value / 10;
		}
		codes[i] = Poscode(text);
	}
	return codes;
}

void deleteCodes(Poscode *codes){
	if (codes != nullptr){
		delete[] codes;
	}
}

Poscode *copyCodes(Poscode *A, size_t n){
	Poscode *B = new Poscode[n];
	for (size_t i = 0; i < n; i++){
		B[i] = A[i];
	}
	return B;
}

bool isSorted(Poscode *A, size_t n){
	for (size_t i = 1; i < n; i++){
		if (A[i - 1].getData() > A[i].getData()){
			return false;
		}
	}
	return true;
}

void printCodes(Poscode *A, size_t n){
	for (size_t i = 0; i < n; i++){
		std::cout << A[i].getData() << " ";
	}
	std::cout << std::endl;
}

int getRandomInt(int min, int max){
	float a = rand() / static_cast<float>(RAND_MAX);
	return static_cast<int>(a * (max - min) + min + 0.5);
}
