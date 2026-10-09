#include "tictactoe/tictactoe.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// Cada prueba marca [ok] o [FALLA]; cuando falla, indica el motivo.
//
// Para revisar, este archivo NO usa turno(), gana() ni sucesores(): el turno,
// los tres en linea y la legalidad de una jugada se revisan con funciones
// propias (mas abajo, turnoRef, ganaRef y jugadaLegal). De las funciones del
// laboratorio solo se usan idTablero y tableroDe, para traducir entre
// tableros e ids.

static int nOk = 0;
static int nFalla = 0;

static void check(const std::string &nombre, bool ok, const std::string &motivo){
	std::cout << "  " << std::left << std::setw(54) << nombre
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

static std::string comillas(const std::string &t){
	return "\"" + t + "\"";
}

static std::string caracter(char ch){
	return std::string("'") + ch + "'";
}

// ---------------------------------------------------------------------------
// Referencias propias del test
// ---------------------------------------------------------------------------

static const int LINEAS[8][3] = {
	{0, 1, 2}, {3, 4, 5}, {6, 7, 8},     // filas
	{0, 3, 6}, {1, 4, 7}, {2, 5, 8},     // columnas
	{0, 4, 8}, {2, 4, 6}                 // diagonales
};

// true si t es un texto de CELDAS caracteres '.', 'X' u 'O'.
static bool esTablero(const std::string &t){
	if ((int)t.size() != CELDAS){
		return false;
	}
	for (int i = 0; i < CELDAS; i++){
		if (t[i] != '.' && t[i] != 'X' && t[i] != 'O'){
			return false;
		}
	}
	return true;
}

static int cuenta(const std::string &t, char ch){
	int n = 0;
	for (int i = 0; i < CELDAS; i++){
		if (t[i] == ch){
			n++;
		}
	}
	return n;
}

static char turnoRef(const std::string &t){
	return (cuenta(t, 'X') == cuenta(t, 'O')) ? 'X' : 'O';
}

static bool ganaRef(const std::string &t, char jugador){
	for (int k = 0; k < 8; k++){
		if (t[LINEAS[k][0]] == jugador && t[LINEAS[k][1]] == jugador && t[LINEAS[k][2]] == jugador){
			return true;
		}
	}
	return false;
}

static bool terminado(const std::string &t){
	return ganaRef(t, 'X') || ganaRef(t, 'O') || cuenta(t, '.') == 0;
}

static std::vector<std::string> sucesoresRef(const std::string &t){
	std::vector<std::string> v;
	if (terminado(t)){
		return v;
	}
	char j = turnoRef(t);
	for (int i = 0; i < CELDAS; i++){
		if (t[i] == '.'){
			std::string s = t;
			s[i] = j;
			v.push_back(s);
		}
	}
	return v;
}

// La celda en que t1 y t2 difieren, o -1 si no difieren en exactamente una.
static int celdaJugada(const std::string &t1, const std::string &t2){
	int celda = -1;
	for (int i = 0; i < CELDAS; i++){
		if (t1[i] != t2[i]){
			if (celda != -1){
				return -1;
			}
			celda = i;
		}
	}
	return celda;
}

// true si t2 se obtiene de t1 con una jugada: la marca del jugador de turno en
// una celda vacia. No revisa si la partida en t1 ya termino.
static bool jugadaLegal(const std::string &t1, const std::string &t2){
	int i = celdaJugada(t1, t2);
	return i != -1 && t1[i] == '.' && t2[i] == turnoRef(t1);
}

// Todos los textos de CELDAS caracteres '.', 'X' u 'O': 3^9 = 19683
// tableros, puedan aparecer en una partida o no.
static std::vector<std::string> TODOS;

static void armarTableros(){
	const char SIMBOLOS[3] = {'.', 'X', 'O'};
	int total = 1;
	for (int i = 0; i < CELDAS; i++){
		total = total * 3;
	}
	for (int k = 0; k < total; k++){
		std::string t(CELDAS, '.');
		int x = k;
		for (int i = 0; i < CELDAS; i++){
			t[i] = SIMBOLOS[x % 3];
			x = x / 3;
		}
		TODOS.push_back(t);
	}
}

// ===========================================================================
// Parte 1 - El sistema de ids
// ===========================================================================

static void testIds(){
	parte("Parte 1 - El sistema de ids");
	int n = cantidadIds();
	int total = TODOS.size();
	std::vector<int> ids(total);
	for (int k = 0; k < total; k++){
		ids[k] = idTablero(TODOS[k]);
	}

	bool ok = (n > 0);
	std::string motivo = ok ? "" : "cantidadIds() retorna " + std::to_string(n);
	for (int k = 0; ok && k < total; k++){
		if (ids[k] < 0 || ids[k] >= n){
			ok = false;
			motivo = "idTablero(" + comillas(TODOS[k]) + ") = " + std::to_string(ids[k])
			       + ", fuera de [0, " + std::to_string(n) + ")";
		}
	}
	check("todo id esta en [0, cantidadIds())", ok, motivo);

	// Se ordenan los pares (id, tablero): dos tableros con el mismo id quedan
	// juntos.
	std::vector<std::pair<int, int> > orden(total);
	for (int k = 0; k < total; k++){
		orden[k] = std::make_pair(ids[k], k);
	}
	std::sort(orden.begin(), orden.end());
	ok = true;
	motivo = "";
	for (int k = 1; ok && k < total; k++){
		if (orden[k].first == orden[k - 1].first){
			ok = false;
			motivo = comillas(TODOS[orden[k - 1].second]) + " y " + comillas(TODOS[orden[k].second])
			       + " tienen el mismo id (" + std::to_string(orden[k].first) + ")";
		}
	}
	check("dos tableros distintos no comparten id", ok, motivo);

	ok = true;
	motivo = "";
	for (int k = 0; ok && k < total; k++){
		std::string t = tableroDe(ids[k]);
		if (t != TODOS[k]){
			ok = false;
			motivo = "tableroDe(idTablero(" + comillas(TODOS[k]) + ")) entrega " + comillas(t);
		}
	}
	check("tableroDe(idTablero(t)) entrega t", ok, motivo);
}

// ===========================================================================
// Parte 2 - El turno, la meta y los sucesores
// ===========================================================================

// El turno solo se revisa en los tableros con tantas X como O, o con una X
// mas: en los demas no hay un turno que tenga sentido.
static void testTurno(){
	bool ok = true;
	std::string motivo;
	int casos = 0;
	for (int k = 0; k < (int)TODOS.size(); k++){
		const std::string &t = TODOS[k];
		int d = cuenta(t, 'X') - cuenta(t, 'O');
		if (d != 0 && d != 1){
			continue;
		}
		casos++;
		char j = turno(idTablero(t));
		if (ok && j != turnoRef(t)){
			ok = false;
			motivo = "turno(idTablero(" + comillas(t) + ")) retorna " + caracter(j)
			       + " y le toca a " + caracter(turnoRef(t));
		}
	}
	check("turno en " + std::to_string(casos) + " tableros", ok, motivo);
}

static void testGana(){
	bool ok = true;
	std::string motivo;
	const char JUGADORES[2] = {'X', 'O'};
	for (int k = 0; ok && k < (int)TODOS.size(); k++){
		const std::string &t = TODOS[k];
		int id = idTablero(t);
		for (int j = 0; ok && j < 2; j++){
			bool g = gana(id, JUGADORES[j]);
			if (g != ganaRef(t, JUGADORES[j])){
				ok = false;
				motivo = "gana(idTablero(" + comillas(t) + "), " + caracter(JUGADORES[j]) + ") retorna "
				       + (g ? "true" : "false");
			}
		}
	}
	check("gana(id, 'X') y gana(id, 'O') en " + std::to_string(TODOS.size()) + " tableros", ok, motivo);
}

static void testEsMeta(){
	bool ok = true;
	std::string motivo;
	for (int k = 0; ok && k < (int)TODOS.size(); k++){
		const std::string &t = TODOS[k];
		bool m = esMeta(idTablero(t));
		if (m != ganaRef(t, 'X')){
			ok = false;
			motivo = "esMeta(idTablero(" + comillas(t) + ")) retorna " + (m ? "true" : "false");
		}
	}
	check("esMeta en " + std::to_string(TODOS.size()) + " tableros", ok, motivo);
}

static std::string conjunto(std::vector<std::string> v){
	std::sort(v.begin(), v.end());
	std::string s = "{";
	for (int i = 0; i < (int)v.size(); i++){
		s = s + (i > 0 ? " " : "") + v[i];
	}
	return s + "}";
}

// Compara los sucesores de t como conjunto: el orden no importa.
static bool sucesoresOk(const std::string &t, std::string &motivo){
	const int GUARDA = -123456;
	int suc[MAX_SUCESORES + 4];
	for (int i = 0; i < MAX_SUCESORES + 4; i++){
		suc[i] = GUARDA;
	}
	int k = sucesores(idTablero(t), suc);
	if (k < 0 || k > MAX_SUCESORES){
		motivo = "sucesores de " + t + " retorna " + std::to_string(k);
		return false;
	}
	for (int i = MAX_SUCESORES; i < MAX_SUCESORES + 4; i++){
		if (suc[i] != GUARDA){
			motivo = "sucesores escribe mas alla de suc[MAX_SUCESORES - 1]";
			return false;
		}
	}
	std::vector<std::string> obtenidos;
	for (int i = 0; i < k; i++){
		obtenidos.push_back(tableroDe(suc[i]));
	}
	std::vector<std::string> esperados = sucesoresRef(t);
	std::sort(obtenidos.begin(), obtenidos.end());
	std::sort(esperados.begin(), esperados.end());
	if (obtenidos != esperados){
		motivo = "de " + t + " se esperaba " + conjunto(esperados) + " y se obtuvo " + conjunto(obtenidos);
		return false;
	}
	return true;
}

static void testSucesores(const std::string &t){
	std::string motivo;
	bool ok = sucesoresOk(t, motivo);
	check("sucesores de " + t, ok, motivo);
}

// Un tablero en que la partida sigue, y el mismo tablero con una jugada mas
// que la termina: el primero tiene sucesores y el segundo no.
static void testSucesoresFin(const std::string &sigue, const std::string &termino, const std::string &porque){
	std::string motivo;
	bool ok = sucesoresOk(sigue, motivo) && sucesoresOk(termino, motivo);
	check("sucesores de " + sigue + " y de " + termino + " (" + porque + ")", ok, motivo);
}

static void testReglas(){
	parte("Parte 2 - El turno, la meta y los sucesores");
	testTurno();
	testGana();
	testEsMeta();
	testSucesores(".........");
	testSucesores("X.O.O.X..");
	testSucesores("X.O.O.X.X");
	testSucesoresFin("XXOXO....", "XXOXO.O..", "gano O");
	testSucesoresFin("XXOOOXXO.", "XXOOOXXOX", "lleno");
}

// ===========================================================================
// Parte 3 - Cinco tableros iniciales
// ===========================================================================

static const int N_EJEMPLOS = 5;
static const char *EJEMPLOS[N_EJEMPLOS] = {
	"X.O.O.X..",
	"X.O.O.X.X",
	".........",
	".X..O..X.",
	"..OXOX..."
};

// Un camino nunca tiene mas de CELDAS + 1 tableros; el tope sirve para no
// recorrer para siempre una lista mal formada.
static const int TOPE_CAMINO = 1000;

// Revisa que L sea una partida legal desde inicio hasta un tablero en que X
// tiene tres en linea. Si lo es, deja en jugadas la cantidad de jugadas.
static bool revisarCamino(LList *L, const std::string &inicio, std::string &motivo, int &jugadas){
	if (L == nullptr || L->getHead() == nullptr){
		motivo = "resolver retorna nullptr o una lista vacia";
		return false;
	}
	std::string prev;
	int i = 0;
	Node *p = L->getHead();
	while (p != nullptr){
		if (i >= TOPE_CAMINO){
			motivo = "el camino tiene mas de " + std::to_string(TOPE_CAMINO) + " tableros";
			return false;
		}
		std::string t = tableroDe(p->getId());
		if (!esTablero(t)){
			motivo = "el tablero " + std::to_string(i) + " del camino (id " + std::to_string(p->getId())
			       + ") no es un tablero: " + comillas(t);
			return false;
		}
		if (i == 0){
			if (t != inicio){
				motivo = "el camino parte en " + t + " y no en " + inicio;
				return false;
			}
		}
		else{
			if (ganaRef(prev, 'X') || ganaRef(prev, 'O')){
				motivo = "en " + prev + " ya gano " + (ganaRef(prev, 'X') ? "X" : "O") + ", y el camino sigue";
				return false;
			}
			if (cuenta(prev, '.') == 0){
				motivo = prev + " esta lleno, y el camino sigue";
				return false;
			}
			if (!jugadaLegal(prev, t)){
				motivo = "de " + prev + " a " + t + " no hay una jugada valida (le toca a "
				       + std::string(1, turnoRef(prev)) + ")";
				return false;
			}
		}
		prev = t;
		i++;
		p = p->getNext();
	}
	if (!ganaRef(prev, 'X')){
		motivo = "el camino termina en " + prev + ", donde X no tiene tres en linea";
		return false;
	}
	jugadas = i - 1;
	return true;
}

// Dibuja los tableros del camino uno al lado del otro, y debajo las jugadas.
static void imprimirCamino(LList *L, int jugadas){
	std::vector<std::string> v;
	Node *p = L->getHead();
	while (p != nullptr && (int)v.size() < TOPE_CAMINO){
		v.push_back(tableroDe(p->getId()));
		p = p->getNext();
	}
	for (int fila = 0; fila < 3; fila++){
		std::cout << "      ";
		for (int k = 0; k < (int)v.size(); k++){
			std::cout << (k > 0 ? "   " : "") << v[k].substr(fila * 3, 3);
		}
		std::cout << std::endl;
	}
	std::string s;
	for (int k = 1; k < (int)v.size(); k++){
		int i = celdaJugada(v[k - 1], v[k]);
		s = s + (k > 1 ? ", " : "") + v[k][i] + " en " + std::to_string(i);
	}
	std::cout << "      " << s << "  (" << jugadas << (jugadas == 1 ? " jugada)" : " jugadas)") << std::endl;
}

static void testEjemplos(){
	parte("Parte 3 - Cinco tableros iniciales");
	for (int k = 0; k < N_EJEMPLOS; k++){
		std::string inicio = EJEMPLOS[k];
		LList *L = resolver(idTablero(inicio));
		std::string motivo;
		int jugadas = 0;
		bool ok = revisarCamino(L, inicio, motivo, jugadas);
		std::cout << std::endl;
		check("inicio " + std::to_string(k + 1) + ": " + inicio + " (juega " + turnoRef(inicio) + ")", ok, motivo);
		if (ok){
			imprimirCamino(L, jugadas);
		}
		delete L;
	}
}

// ===========================================================================
// Parte 4 - Tableros sin solucion
// ===========================================================================

// Cada par es un tablero desde el que X todavia puede ganar y el mismo
// tablero con una jugada mas de O, desde el que ya no puede.
struct Par{
	const char *conCamino;
	const char *sinCamino;
	const char *porque;
};

static const int N_PARES = 2;
static const Par PARES[N_PARES] = {
	{"XXOXO....", "XXOXO.O..", "ya gano O"},
	{"XXOO..X..", "XXOOO.X..", "X ya no puede ganar"}
};

static void testSinSolucion(){
	parte("Parte 4 - Tableros sin solucion");
	for (int k = 0; k < N_PARES; k++){
		std::string con = PARES[k].conCamino;
		std::string sin = PARES[k].sinCamino;
		LList *L1 = resolver(idTablero(con));
		LList *L2 = resolver(idTablero(sin));
		std::string motivo;
		int jugadas = 0;
		bool ok = revisarCamino(L1, con, motivo, jugadas);
		if (!ok){
			motivo = "desde " + con + ": " + motivo;
		}
		else if (L2 != nullptr){
			ok = false;
			motivo = "desde " + sin + " no hay camino y resolver no retorna nullptr";
		}
		std::cout << std::endl;
		check("desde " + con + " si hay camino; desde " + sin + ", no", ok, motivo);
		if (ok){
			imprimirCamino(L1, jugadas);
			std::cout << "      desde " << sin << ": nullptr (" << PARES[k].porque << ")" << std::endl;
		}
		delete L1;
		delete L2;
	}
}

// ---------------------------------------------------------------------------

int main(int nargs, char **vargs){
	std::cout << "Laboratorio 7 - Tic-tac-toe" << std::endl;
	armarTableros();

	testIds();
	testReglas();
	testEjemplos();
	testSinSolucion();

	std::cout << std::endl << "Resumen: " << nOk << " de " << (nOk + nFalla)
	          << " correctas" << std::endl;
	return 0;
}
