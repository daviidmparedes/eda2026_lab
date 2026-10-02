#include "misioneros/misioneros.hpp"

#include <iomanip>
#include <iostream>
#include <string>

// Cada prueba marca [ok] o [FALLA]; cuando falla, indica el motivo.
//
// Para revisar, este archivo NO usa valido() ni sucesores(): la regla de las
// orillas y la legalidad de un cruce se revisan con funciones propias (mas
// abajo, reglaOk y cruceLegal). De las funciones del laboratorio solo se usan
// idEstado y estadoDe, para traducir entre estados e ids.

static int nOk = 0;
static int nFalla = 0;

static void check(const std::string &nombre, bool ok, const std::string &motivo){
	std::cout << "  " << std::left << std::setw(46) << nombre
	          << (ok ? "[ok]" : "[FALLA]") << std::endl;
	if (!ok && !motivo.empty()){
		std::cout << "      " << motivo << std::endl;
	}
	if (ok){ nOk++; }
	else{ nFalla++; }
}

static void parte(const char *titulo){
	std::cout << std::endl << "==== " << titulo << std::endl;
}

static std::string txt(int m, int c, int b){
	return "(" + std::to_string(m) + "," + std::to_string(c) + "," + std::to_string(b) + ")";
}

// ---------------------------------------------------------------------------
// Referencias propias del test
// ---------------------------------------------------------------------------

static bool enRango(int m, int c, int b){
	return m >= 0 && m <= MISIONEROS && c >= 0 && c <= CANIBALES && (b == 0 || b == 1);
}

static bool reglaOk(int m, int c){
	if (m < 0 || m > MISIONEROS || c < 0 || c > CANIBALES){
		return false;
	}
	if (m > 0 && c > m){
		return false;
	}
	int md = MISIONEROS - m;
	int cd = CANIBALES - c;
	return !(md > 0 && cd > md);
}

// true si (m2, c2, b2) se obtiene de (m1, c1, b1) con un cruce: el bote cambia
// de orilla y lleva entre 1 y CAPACIDAD_BOTE personas desde la orilla en que
// estaba.
static bool cruceLegal(int m1, int c1, int b1, int m2, int c2, int b2){
	if (b2 != 1 - b1){
		return false;
	}
	int dm = (b1 == 1) ? m1 - m2 : m2 - m1;
	int dc = (b1 == 1) ? c1 - c2 : c2 - c1;
	if (dm < 0 || dc < 0){
		return false;
	}
	int personas = dm + dc;
	return personas >= 1 && personas <= CAPACIDAD_BOTE;
}

static std::string personas(int dm, int dc){
	std::string s;
	if (dm > 0){
		s = std::to_string(dm) + (dm == 1 ? " misionero" : " misioneros");
	}
	if (dc > 0){
		if (!s.empty()){ s = s + " y "; }
		s = s + std::to_string(dc) + (dc == 1 ? " canibal" : " canibales");
	}
	return s;
}

static std::string describirCruce(int m1, int c1, int b1, int m2, int c2){
	if (b1 == 1){
		return "a la derecha: " + personas(m1 - m2, c1 - c2);
	}
	return "a la izquierda: " + personas(m2 - m1, c2 - c1);
}

// Todos los estados (m, c, b) posibles, validos o no.
static const int N_ESTADOS = (MISIONEROS + 1) * (CANIBALES + 1) * 2;
static int EM[N_ESTADOS];
static int EC[N_ESTADOS];
static int EB[N_ESTADOS];

static void armarEstados(){
	int k = 0;
	for (int m = 0; m <= MISIONEROS; m++){
		for (int c = 0; c <= CANIBALES; c++){
			for (int b = 0; b <= 1; b++){
				EM[k] = m;
				EC[k] = c;
				EB[k] = b;
				k++;
			}
		}
	}
}

// ===========================================================================
// Parte 1 - El sistema de ids
// ===========================================================================

static void testIds(){
	parte("Parte 1 - El sistema de ids");
	int n = cantidadIds();

	bool ok = (n > 0);
	std::string motivo = ok ? "" : "cantidadIds() retorna " + std::to_string(n);
	for (int k = 0; ok && k < N_ESTADOS; k++){
		int id = idEstado(EM[k], EC[k], EB[k]);
		if (id < 0 || id >= n){
			ok = false;
			motivo = "idEstado" + txt(EM[k], EC[k], EB[k]) + " = " + std::to_string(id)
			       + ", fuera de [0, " + std::to_string(n) + ")";
		}
	}
	check("todo id esta en [0, cantidadIds())", ok, motivo);

	ok = true;
	motivo = "";
	for (int i = 0; ok && i < N_ESTADOS; i++){
		int idI = idEstado(EM[i], EC[i], EB[i]);
		for (int j = i + 1; ok && j < N_ESTADOS; j++){
			if (idEstado(EM[j], EC[j], EB[j]) == idI){
				ok = false;
				motivo = txt(EM[i], EC[i], EB[i]) + " y " + txt(EM[j], EC[j], EB[j])
				       + " tienen el mismo id (" + std::to_string(idI) + ")";
			}
		}
	}
	check("dos estados distintos no comparten id", ok, motivo);

	ok = true;
	motivo = "";
	for (int k = 0; ok && k < N_ESTADOS; k++){
		int m = -1;
		int c = -1;
		int b = -1;
		estadoDe(idEstado(EM[k], EC[k], EB[k]), m, c, b);
		if (m != EM[k] || c != EC[k] || b != EB[k]){
			ok = false;
			motivo = "estadoDe(idEstado" + txt(EM[k], EC[k], EB[k]) + ") entrega " + txt(m, c, b);
		}
	}
	check("estadoDe(idEstado(e)) entrega e", ok, motivo);
}

// ===========================================================================
// Parte 2 - La regla y los sucesores
// ===========================================================================

static void testValido(){
	bool ok = true;
	std::string motivo;
	int casos = 0;
	for (int m = -1; m <= MISIONEROS + 1; m++){
		for (int c = -1; c <= CANIBALES + 1; c++){
			casos++;
			if (ok && valido(m, c) != reglaOk(m, c)){
				ok = false;
				motivo = "valido(" + std::to_string(m) + ", " + std::to_string(c) + ") retorna "
				       + (valido(m, c) ? "true" : "false");
			}
		}
	}
	check("valido(m, c) en " + std::to_string(casos) + " casos", ok, motivo);
}

// La meta es (0,0,0). El estado (0,0,1) no se revisa: no es alcanzable, asi
// que da lo mismo si esMeta lo acepta.
static void testEsMeta(){
	bool ok = esMeta(idEstado(0, 0, 0));
	std::string motivo = ok ? "" : "esMeta(idEstado(0,0,0)) retorna false";
	for (int k = 0; ok && k < N_ESTADOS; k++){
		if (EM[k] == 0 && EC[k] == 0){
			continue;
		}
		if (esMeta(idEstado(EM[k], EC[k], EB[k]))){
			ok = false;
			motivo = "esMeta(idEstado" + txt(EM[k], EC[k], EB[k]) + ") retorna true";
		}
	}
	check("esMeta solo acepta la meta (0,0,0)", ok, motivo);
}

// Compara los sucesores como conjunto: el orden no importa.
static void testSucesores(int m, int c, int b, const int esperados[][3], int nEsperados){
	const int GUARDA = -123456;
	int suc[MAX_SUCESORES + 4];
	for (int i = 0; i < MAX_SUCESORES + 4; i++){
		suc[i] = GUARDA;
	}
	int k = sucesores(idEstado(m, c, b), suc);

	bool ok = true;
	std::string motivo;
	if (k < 0 || k > MAX_SUCESORES){
		ok = false;
		motivo = "sucesores retorna " + std::to_string(k);
	}
	for (int i = MAX_SUCESORES; ok && i < MAX_SUCESORES + 4; i++){
		if (suc[i] != GUARDA){
			ok = false;
			motivo = "sucesores escribe mas alla de suc[MAX_SUCESORES - 1]";
		}
	}

	std::string obtenidos;
	bool usado[MAX_SUCESORES] = {false, false, false, false, false};
	for (int i = 0; ok && i < k; i++){
		int sm = -1;
		int sc = -1;
		int sb = -1;
		estadoDe(suc[i], sm, sc, sb);
		obtenidos = obtenidos + (i > 0 ? " " : "") + txt(sm, sc, sb);
		bool encontrado = false;
		for (int j = 0; j < nEsperados && !encontrado; j++){
			if (!usado[j] && esperados[j][0] == sm && esperados[j][1] == sc && esperados[j][2] == sb){
				usado[j] = true;
				encontrado = true;
			}
		}
		if (!encontrado){
			ok = false;
		}
	}
	if (ok && k != nEsperados){
		ok = false;
	}
	if (!ok && motivo.empty()){
		std::string esperadosTxt;
		for (int j = 0; j < nEsperados; j++){
			esperadosTxt = esperadosTxt + (j > 0 ? " " : "") + txt(esperados[j][0], esperados[j][1], esperados[j][2]);
		}
		motivo = "se esperaba {" + esperadosTxt + "} y se obtuvo {" + obtenidos + "}";
	}
	check("sucesores de " + txt(m, c, b), ok, motivo);
}

static void testReglas(){
	parte("Parte 2 - La regla, la meta y los sucesores");
	testValido();
	testEsMeta();
	const int de331[3][3] = {{3, 2, 0}, {3, 1, 0}, {2, 2, 0}};
	const int de220[2][3] = {{3, 2, 1}, {3, 3, 1}};
	const int de110[2][3] = {{3, 1, 1}, {2, 2, 1}};
	testSucesores(3, 3, 1, de331, 3);
	testSucesores(2, 2, 0, de220, 2);
	testSucesores(1, 1, 0, de110, 2);
}

// ===========================================================================
// Parte 3 - Cinco estados iniciales
// ===========================================================================

struct Ejemplo{
	int m0, c0, b0;     // estado inicial
	int m1, c1, b1;     // la meta
};

static const Ejemplo EJEMPLOS[5] = {
	{3, 3, 1,   0, 0, 0},
	{3, 2, 0,   0, 0, 0},
	{2, 2, 0,   0, 0, 0},
	{1, 1, 0,   0, 0, 0},
	{0, 3, 1,   0, 0, 0}
};

// Un camino nunca necesita mas estados que este tope; sirve para no recorrer
// para siempre una lista mal formada.
static const int TOPE_CAMINO = 1000;

// Revisa que L sea un camino legal desde el estado inicial hasta la meta. Si
// lo es, deja en cruces la cantidad de cruces.
static bool revisarCamino(LList *L, const Ejemplo &ej, std::string &motivo, int &cruces){
	if (L == nullptr || L->getHead() == nullptr){
		motivo = "resolver retorna nullptr o una lista vacia";
		return false;
	}
	int pm = 0, pc = 0, pb = 0;
	int i = 0;
	Node *p = L->getHead();
	while (p != nullptr){
		if (i >= TOPE_CAMINO){
			motivo = "el camino tiene mas de " + std::to_string(TOPE_CAMINO) + " estados";
			return false;
		}
		int m = -1, c = -1, b = -1;
		estadoDe(p->getId(), m, c, b);
		if (!enRango(m, c, b)){
			motivo = "el estado " + std::to_string(i) + " del camino (id " + std::to_string(p->getId())
			       + ") no corresponde a un estado (m, c, b)";
			return false;
		}
		if (!reglaOk(m, c)){
			motivo = "el estado " + std::to_string(i) + " del camino, " + txt(m, c, b) + ", no respeta la regla";
			return false;
		}
		if (i == 0){
			if (m != ej.m0 || c != ej.c0 || b != ej.b0){
				motivo = "el camino parte en " + txt(m, c, b) + " y no en " + txt(ej.m0, ej.c0, ej.b0);
				return false;
			}
		}
		else if (!cruceLegal(pm, pc, pb, m, c, b)){
			motivo = "de " + txt(pm, pc, pb) + " a " + txt(m, c, b) + " no hay un cruce valido";
			return false;
		}
		pm = m;
		pc = c;
		pb = b;
		i++;
		p = p->getNext();
	}
	if (pm != ej.m1 || pc != ej.c1 || pb != ej.b1){
		motivo = "el camino termina en " + txt(pm, pc, pb) + " y no en " + txt(ej.m1, ej.c1, ej.b1);
		return false;
	}
	cruces = i - 1;
	return true;
}

static void imprimirCamino(LList *L){
	int pm = 0, pc = 0, pb = 0;
	int i = 0;
	Node *p = L->getHead();
	while (p != nullptr && i < TOPE_CAMINO){
		int m = -1, c = -1, b = -1;
		estadoDe(p->getId(), m, c, b);
		std::cout << "      " << std::left << std::setw(10) << txt(m, c, b);
		if (i > 0){
			std::cout << "bote " << describirCruce(pm, pc, pb, m, c);
		}
		std::cout << std::endl;
		pm = m;
		pc = c;
		pb = b;
		i++;
		p = p->getNext();
	}
}

static void testEjemplos(){
	parte("Parte 3 - Cinco estados iniciales");
	for (int k = 0; k < 5; k++){
		const Ejemplo &ej = EJEMPLOS[k];
		std::string a = txt(ej.m0, ej.c0, ej.b0);
		std::string b = txt(ej.m1, ej.c1, ej.b1);
		LList *L = resolver(idEstado(ej.m0, ej.c0, ej.b0));
		std::string motivo;
		int cruces = 0;
		bool ok = revisarCamino(L, ej, motivo, cruces);
		std::cout << std::endl;
		check("inicio " + std::to_string(k + 1) + ": de " + a + " a la meta " + b, ok, motivo);
		if (ok){
			imprimirCamino(L);
			std::cout << "      " << cruces << " cruces" << std::endl;
		}
		delete L;
	}
}

// ---------------------------------------------------------------------------

int main(int nargs, char **vargs){
	std::cout << "Laboratorio 6 - Misioneros y canibales" << std::endl;
	armarEstados();

	testIds();
	testReglas();
	testEjemplos();

	std::cout << std::endl << "Resumen: " << nOk << " de " << (nOk + nFalla)
	          << " correctas" << std::endl;
	return 0;
}
