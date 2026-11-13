#include "puzzle/estado.hpp"
#include "puzzle/puzzle.hpp"

#include <algorithm>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// Laboratorio 10 - Puzzle 3x3 (Control 3)
//
// Uso:
//   ./test              corre todas las pruebas
//   ./test 123045786    resuelve solo ese tablero, con la Cola y con el
//                       MaxHeap, y compara con la respuesta correcta
//
// Para revisar, este archivo no usa tu Estado::mover: aplica las acciones con
// su propia funcion (moverRef) y calcula la solucion mas corta de cada
// tablero con su propia busqueda (calcularDistancias).

static int nOk = 0;
static int nFalla = 0;

// El nombre se imprime antes de correr la prueba: si el programa se cae o no
// termina, la ultima linea dice en que prueba fue.
static void empezar(const std::string &nombre){
	std::cout << "  " << std::left << std::setw(54) << nombre << std::flush;
}

static void terminar(bool ok, const std::string &motivo){
	std::cout << (ok ? "[ok]" : "[FALLA]") << std::endl;
	if (!ok && !motivo.empty()){
		std::cout << "      " << motivo << std::endl;
	}
	if (ok){ nOk++; }
	else{ nFalla++; }
}

static void check(const std::string &nombre, bool ok, const std::string &motivo){
	empezar(nombre);
	terminar(ok, motivo);
}

static void parte(const std::string &titulo){
	std::cout << std::endl << "==== " << titulo << std::endl;
}

static std::string num(long long x){
	return std::to_string(x);
}

// "1 movimiento", "3 movimientos".
static std::string cuantos(long long x, const std::string &palabra){
	return num(x) + " " + palabra + (x == 1 ? "" : "s");
}

// ===========================================================================
// Referencia
// ===========================================================================

// Cuantos tableros se alcanzan desde cualquier tablero: la mitad de 9!.
static const int ALCANZABLES = 181440;

static char opuesta(char a){
	if (a == 'N'){ return 'S'; }
	if (a == 'S'){ return 'N'; }
	if (a == 'E'){ return 'O'; }
	return 'E';
}

// El tablero que resulta de aplicar la accion, o "" si no se puede.
static std::string moverRef(const std::string &t, char accion){
	int h = t.find('0');
	int fila = h / 3;
	int col = h % 3;
	if (accion == 'N'){ fila--; }
	else if (accion == 'S'){ fila++; }
	else if (accion == 'E'){ col++; }
	else if (accion == 'O'){ col--; }
	else{ return ""; }
	if (fila < 0 || fila > 2 || col < 0 || col > 2){
		return "";
	}
	std::string s = t;
	std::swap(s[h], s[fila * 3 + col]);
	return s;
}

static int prioridadRef(const std::string &t){
	int k = 0;
	for (int i = 0; i < 9; i++){
		if (t[i] != '0' && t[i] == META[i]){
			k++;
		}
	}
	return k;
}

// La distancia de cada tablero a la META, con una busqueda en anchura que
// parte desde la META. Los tableros que no estan no tienen solucion.
static std::unordered_map<std::string, int> distancias;

static void calcularDistancias(){
	if (!distancias.empty()){
		return;
	}
	std::deque<std::string> q;
	distancias[META] = 0;
	q.push_back(META);
	while (!q.empty()){
		std::string t = q.front();
		q.pop_front();
		for (int i = 0; i < 4; i++){
			std::string s = moverRef(t, ACCIONES[i]);
			if (!s.empty() && distancias.count(s) == 0){
				distancias[s] = distancias[t] + 1;
				q.push_back(s);
			}
		}
	}
}

// -1 si el tablero no tiene solucion.
static int distancia(const std::string &t){
	calcularDistancias();
	auto it = distancias.find(t);
	return it == distancias.end() ? -1 : it->second;
}

static bool tableroValido(const std::string &t){
	if (t.size() != 9){
		return false;
	}
	std::string orden = t;
	std::sort(orden.begin(), orden.end());
	return orden == "012345678";
}

// Aplica las acciones desde el inicio. Retorna el tablero final, o "" si
// alguna accion no se puede hacer (y entonces k es su posicion).
static std::string aplicarTodas(const std::string &inicio, const std::string &acciones, int &k){
	std::string t = inicio;
	for (k = 0; k < (int)acciones.size(); k++){
		t = moverRef(t, acciones[k]);
		if (t.empty()){
			return "";
		}
	}
	return t;
}

static bool llegaALaMeta(const std::string &inicio, const std::string &acciones){
	int k;
	return aplicarTodas(inicio, acciones, k) == META;
}

// Revisa que las acciones lleven del inicio a la META. Si no, explica por que.
static bool revisarAcciones(const std::string &inicio, const std::string &acciones, std::string &motivo){
	for (char a : acciones){
		if (a != 'N' && a != 'S' && a != 'E' && a != 'O'){
			motivo = std::string("la accion '") + a + "' no existe: las acciones son N, S, E y O";
			return false;
		}
	}
	int k;
	std::string fin = aplicarTodas(inicio, acciones, k);
	if (fin == META){
		return true;
	}

	std::string invertidas = acciones;
	for (char &a : invertidas){
		a = opuesta(a);
	}
	std::string reves(acciones.rbegin(), acciones.rend());
	if (llegaALaMeta(inicio, invertidas)){
		motivo = "las acciones estan al reves: 'N' mueve la ficha que esta al norte del hueco (el hueco sube)";
	}
	else if (llegaALaMeta(inicio, reves)){
		motivo = "las acciones estan en orden inverso: la primera tiene que ser la que se aplica al tablero inicial";
	}
	else if (fin.empty()){
		motivo = "la accion " + num(k + 1) + " ('" + acciones[k] + "') no se puede hacer: no hay ficha de ese lado del hueco";
	}
	else{
		motivo = "las acciones (" + acciones + ") terminan en " + fin + ", que no es la meta";
	}
	return false;
}

// Revisa el resultado de una busqueda sobre el tablero t.
static bool revisarResultado(const std::string &t, const Resultado &r, bool conCola, std::string &motivo){
	int d = distancia(t);
	if (d < 0){
		if (r.encontrado){
			motivo = "dice que encontro una solucion, y este tablero no tiene solucion";
			return false;
		}
		if (r.explorados < ALCANZABLES){
			motivo = "dice que no hay solucion, pero solo saco " + num(r.explorados)
			       + " estados de la frontera: para saberlo hay que revisar los "
			       + num(ALCANZABLES) + " tableros alcanzables";
			return false;
		}
		return true;
	}
	if (!r.encontrado){
		motivo = "no encontro solucion, y la mas corta tiene " + cuantos(d, "movimiento");
		return false;
	}
	if (!revisarAcciones(t, r.acciones, motivo)){
		return false;
	}
	if (conCola && (int)r.acciones.size() != d){
		motivo = "la solucion tiene " + cuantos(r.acciones.size(), "movimiento") + " y la mas corta tiene "
		       + num(d) + ": con una Cola tiene que salir la mas corta";
		return false;
	}
	return true;
}

// ===========================================================================
// Parte 1 - Estado
// ===========================================================================

static std::string txt(Estado *e){
	return e == nullptr ? "nullptr" : e->getTablero();
}

// Compara mover(accion) con la referencia. Si el resultado es el de la accion
// opuesta, lo dice.
static bool moverBien(const std::string &t, char accion, std::string &motivo){
	Estado e(t);
	Estado *s = e.mover(accion);
	std::string esperado = moverRef(t, accion);
	bool ok;
	if (esperado.empty()){
		ok = s == nullptr;
	}
	else{
		ok = s != nullptr && s->getTablero() == esperado;
	}
	if (!ok){
		std::string opuesto = moverRef(t, opuesta(accion));
		motivo = std::string("mover('") + accion + "') desde " + t + " entrega " + txt(s)
		       + " y deberia entregar " + (esperado.empty() ? "nullptr" : esperado);
		if (s != nullptr && !opuesto.empty() && s->getTablero() == opuesto){
			motivo = motivo + ". Es lo que deberia hacer '" + opuesta(accion)
			       + "': 'N' mueve la ficha que esta al norte del hueco (el hueco sube)";
		}
	}
	delete s;
	return ok;
}

static void testEstado(){
	parte("Parte 1 - Estado");

	// esMeta
	const std::string noMetas[8] = {"123456708", "023456781", "213045786", "123456870",
	                                "012345678", "876543210", "123405786", "823456710"};
	Estado meta(META);
	bool ok = meta.esMeta();
	std::string motivo = ok ? "" : "esMeta() de la meta retorna false";
	for (int i = 0; ok && i < 8; i++){
		Estado e(noMetas[i]);
		if (e.esMeta()){
			ok = false;
			motivo = "esMeta() de " + noMetas[i] + " retorna true";
		}
	}
	check("esMeta: la meta y 8 tableros que no lo son", ok, motivo);

	// prioridad
	Estado ejemplo("213045786");
	check("prioridad del ejemplo del enunciado (213045786): 3", ejemplo.prioridad() == 3,
	      "retorna " + num(ejemplo.prioridad()));
	check("prioridad de la meta: 8", meta.prioridad() == 8,
	      "retorna " + num(meta.prioridad())
	      + (meta.prioridad() == 9 ? " (el hueco no es una ficha: no se cuenta)" : ""));

	std::mt19937 gen(2026);
	std::string t = META;
	ok = true;
	motivo = "";
	for (int i = 0; ok && i < 30; i++){
		std::shuffle(t.begin(), t.end(), gen);
		Estado e(t);
		if (e.prioridad() != prioridadRef(t)){
			ok = false;
			motivo = "prioridad de " + t + " retorna " + num(e.prioridad()) + " y deberia ser " + num(prioridadRef(t));
		}
	}
	check("prioridad de 30 tableros al azar", ok, motivo);

	// mover
	ok = true;
	motivo = "";
	for (int i = 0; ok && i < 4; i++){
		ok = moverBien("123405678", ACCIONES[i], motivo);
	}
	check("mover: las cuatro acciones con el hueco al centro", ok, motivo);

	ok = true;
	motivo = "";
	const std::string bordes[9] = {"012345678", "102345678", "120345678", "123045678", "123405678",
	                               "123450678", "123456078", "123456708", "123456780"};
	for (int i = 0; ok && i < 9; i++){
		for (int j = 0; ok && j < 4; j++){
			ok = moverBien(bordes[i], ACCIONES[j], motivo);
		}
	}
	check("mover: el hueco en cada una de las 9 posiciones", ok, motivo);

	ok = true;
	motivo = "";
	int movidos = 0;
	for (int i = 0; ok && i < 4; i++){
		Estado original("123405678");
		Estado *s = original.mover(ACCIONES[i]);
		if (s != nullptr){
			movidos++;
		}
		delete s;
		if (original.getTablero() != "123405678"){
			ok = false;
			motivo = std::string("despues de mover('") + ACCIONES[i] + "'), el estado original quedo en "
			       + original.getTablero() + " y deberia seguir en 123405678";
		}
	}
	if (ok && movidos == 0){
		ok = false;
		motivo = "mover no funciona (revisa las pruebas anteriores)";
	}
	check("mover no cambia el estado original", ok, motivo);

	// camino
	Estado inicio("123405678");
	ok = inicio.camino() == "";
	motivo = ok ? "" : "el camino del estado inicial es \"" + inicio.camino() + "\" y deberia ser \"\"";
	Estado *uno = inicio.mover('N');
	Estado *dos = uno == nullptr ? nullptr : uno->mover('E');
	Estado *tres = dos == nullptr ? nullptr : dos->mover('S');
	if (ok && tres == nullptr){
		ok = false;
		motivo = "mover no funciona (revisa las pruebas anteriores)";
	}
	else if (ok && tres->camino() != "NES"){
		ok = false;
		motivo = "despues de N, E y S, el camino es \"" + tres->camino() + "\" y deberia ser \"NES\"";
	}
	check("camino: el inicial no tiene acciones, y despues de 3", ok, motivo);
	delete tres;
	delete dos;
	delete uno;
}

// ===========================================================================
// Partes 2 y 3 - Resolver los tableros de data/tableros.txt
// ===========================================================================

static std::vector<std::string> leerTableros(){
	std::vector<std::string> tableros;
	std::ifstream f(DATA_FILE);
	if (!f){
		std::cout << "  no se pudo abrir " << DATA_FILE << std::endl;
		return tableros;
	}
	std::string linea;
	while (std::getline(f, linea)){
		std::istringstream in(linea);
		std::string t;
		if (in >> t && t[0] != '#'){
			if (tableroValido(t)){
				tableros.push_back(t);
			}
			else{
				std::cout << "  tableros.txt: \"" << t << "\" no es un tablero (9 cifras distintas del 0 al 8)" << std::endl;
			}
		}
	}
	return tableros;
}

static std::string nombreTablero(const std::string &t){
	int d = distancia(t);
	if (d < 0){
		return t + " (sin solucion)";
	}
	return t + " (" + cuantos(d, "movimiento") + ")";
}

static void testResolver(const std::string &titulo, const std::vector<std::string> &tableros, bool conCola){
	parte(titulo);
	for (const std::string &t : tableros){
		empezar(nombreTablero(t));
		Resultado r = conCola ? resolverConCola(t) : resolverConHeap(t);
		std::string motivo;
		bool ok = revisarResultado(t, r, conCola, motivo);
		terminar(ok, motivo);
		if (ok && r.encontrado){
			std::cout << "      " << cuantos(r.acciones.size(), "movimiento") << ", "
			          << cuantos(r.explorados, "estado") << " explorados" << std::endl;
		}
	}
}

// ===========================================================================
// Parte 4 - Cola contra MaxHeap
// ===========================================================================

static void testComparar(){
	parte("Parte 4 - Cola contra MaxHeap");
	const int N = 20;
	std::mt19937 gen(2026);
	std::string t = META;
	long long largoCola = 0, largoHeap = 0, explCola = 0, explHeap = 0;
	bool ok = true;
	std::string motivo;
	empezar(num(N) + " tableros al azar: las dos busquedas los resuelven");
	for (int i = 0; ok && i < N; i++){
		do{
			std::shuffle(t.begin(), t.end(), gen);
		} while (distancia(t) < 0);
		Resultado c = resolverConCola(t);
		Resultado h = resolverConHeap(t);
		if (!revisarResultado(t, c, true, motivo)){
			ok = false;
			motivo = t + " con la Cola: " + motivo;
		}
		else if (!revisarResultado(t, h, false, motivo)){
			ok = false;
			motivo = t + " con el MaxHeap: " + motivo;
		}
		largoCola += c.acciones.size();
		largoHeap += h.acciones.size();
		explCola += c.explorados;
		explHeap += h.explorados;
	}
	terminar(ok, motivo);

	if (ok){
		// Usando la prioridad, el MaxHeap explora cientos de veces menos que
		// la Cola. Sin usarla (por ejemplo, con la misma prioridad para todos),
		// explora casi lo mismo que la Cola.
		check("el MaxHeap explora 10 veces menos estados que la Cola",
		      explCola > 0 && explHeap * 10 <= explCola,
		      "en promedio, la Cola saca " + num(explCola / N) + " estados de la frontera y el MaxHeap "
		      + num(explHeap / N) + ": revisa que el MaxHeap use la prioridad de cada estado");
		std::cout << std::endl
		          << "      Promedio en " << N << " tableros   Movimientos   Explorados" << std::endl
		          << "      Cola                  " << std::right << std::setw(15) << largoCola / N
		          << std::setw(13) << explCola / N << std::endl
		          << "      MaxHeap               " << std::setw(15) << largoHeap / N
		          << std::setw(13) << explHeap / N << std::endl;
	}
	else{
		check("el MaxHeap explora 10 veces menos estados que la Cola", false,
		      "primero las dos busquedas tienen que resolver los tableros");
	}
}

// ===========================================================================
// ./test <tablero>
// ===========================================================================

static void imprimirTablero(const std::string &t){
	for (int i = 0; i < 9; i += 3){
		std::cout << "      " << t[i] << " " << t[i + 1] << " " << t[i + 2] << std::endl;
	}
}

static void unaBusqueda(const std::string &t, bool conCola){
	std::cout << std::endl << (conCola ? "Con la Cola" : "Con el MaxHeap") << std::endl;
	Resultado r = conCola ? resolverConCola(t) : resolverConHeap(t);
	std::string motivo;
	bool ok = revisarResultado(t, r, conCola, motivo);
	if (r.encontrado){
		std::cout << "  acciones: " << r.acciones << std::endl
		          << "  " << cuantos(r.acciones.size(), "movimiento") << ", "
		          << cuantos(r.explorados, "estado") << " explorados" << std::endl;
	}
	else{
		std::cout << "  no encontro solucion, despues de explorar " << r.explorados << " estados" << std::endl;
	}
	std::cout << "  " << (ok ? "[ok]" : "[FALLA]") << std::endl;
	if (!ok){
		std::cout << "      " << motivo << std::endl;
	}
	else if (r.encontrado && r.acciones.size() <= 31){
		std::string paso = t;
		std::cout << std::endl;
		imprimirTablero(paso);
		for (char a : r.acciones){
			paso = moverRef(paso, a);
			std::cout << "    mover " << a << std::endl;
			imprimirTablero(paso);
		}
	}
}

static int resolverUno(const std::string &t){
	if (!tableroValido(t)){
		std::cout << "\"" << t << "\" no es un tablero: tienen que ser 9 cifras distintas del 0 al 8, "
		          << "leidas por filas (por ejemplo 123045786)" << std::endl;
		return 1;
	}
	std::cout << "Tablero " << t << std::endl;
	imprimirTablero(t);
	int d = distancia(t);
	if (d < 0){
		std::cout << "Respuesta correcta: no tiene solucion." << std::endl;
	}
	else{
		std::cout << "Respuesta correcta: la solucion mas corta tiene " << cuantos(d, "movimiento") << "." << std::endl;
	}
	unaBusqueda(t, true);
	unaBusqueda(t, false);
	return 0;
}

int main(int nargs, char** vargs){
	std::cout << "Laboratorio 10 - Puzzle 3x3" << std::endl;
	if (nargs >= 2){
		return resolverUno(vargs[1]);
	}
	testEstado();
	std::vector<std::string> tableros = leerTableros();
	testResolver("Parte 2 - Resolver con una Cola", tableros, true);
	testResolver("Parte 3 - Resolver con un MaxHeap", tableros, false);
	testComparar();
	std::cout << std::endl << "Resumen: " << nOk << " de " << (nOk + nFalla)
	          << " correctas" << std::endl;
	return 0;
}
