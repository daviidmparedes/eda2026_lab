#include "tictactoe/tictactoe.hpp"
#include "tictactoe/stack.hpp"

// ===========================================================================
// SOLUCION DE REFERENCIA
// ===========================================================================

// El tablero se lee como un numero de 9 cifras en base 3: la celda i es la
// cifra que vale 3^i, con '.' = 0, 'X' = 1 y 'O' = 2. Da ids de 0 a 3^9 - 1.
static const char SIMBOLOS[3] = {'.', 'X', 'O'};

static int valor(char ch){
	if (ch == 'X'){
		return 1;
	}
	if (ch == 'O'){
		return 2;
	}
	return 0;
}

int idTablero(const std::string &t){
	int id = 0;
	int potencia = 1;
	for (int i = 0; i < CELDAS; i++){
		id = id + valor(t[i]) * potencia;
		potencia = potencia * 3;
	}
	return id;
}

// Las cifras salen de la menos significativa (la celda 0) a la mas
// significativa (la celda 8), igual que al escribir un numero en otra base.
std::string tableroDe(int id){
	std::string t(CELDAS, '.');
	for (int i = 0; i < CELDAS; i++){
		t[i] = SIMBOLOS[id % 3];
		id = id / 3;
	}
	return t;
}

int cantidadIds(){
	int n = 1;
	for (int i = 0; i < CELDAS; i++){
		n = n * 3;
	}
	return n;
}

// X parte y los turnos se alternan: si hay tantas X como O le toca a X, y si
// hay una X mas le toca a O.
char turno(int id){
	std::string t = tableroDe(id);
	int nx = 0;
	int no = 0;
	for (int i = 0; i < CELDAS; i++){
		if (t[i] == 'X'){
			nx = nx + 1;
		}
		if (t[i] == 'O'){
			no = no + 1;
		}
	}
	if (nx == no){
		return 'X';
	}
	return 'O';
}

// Las 8 lineas del tablero: 3 filas, 3 columnas y 2 diagonales.
static const int LINEAS[8][3] = {
	{0, 1, 2}, {3, 4, 5}, {6, 7, 8},
	{0, 3, 6}, {1, 4, 7}, {2, 5, 8},
	{0, 4, 8}, {2, 4, 6}
};

bool gana(int id, char jugador){
	std::string t = tableroDe(id);
	for (int k = 0; k < 8; k++){
		if (t[LINEAS[k][0]] == jugador && t[LINEAS[k][1]] == jugador && t[LINEAS[k][2]] == jugador){
			return true;
		}
	}
	return false;
}

bool esMeta(int id){
	return gana(id, 'X');
}

// Si alguien ya gano la partida termino: no hay sucesores. Si no, el jugador
// de turno pone su marca en cada celda vacia, de la 0 a la 8. Un tablero
// lleno no tiene celdas vacias, asi que tampoco tiene sucesores.
int sucesores(int id, int* suc){
	if (gana(id, 'X') || gana(id, 'O')){
		return 0;
	}
	std::string t = tableroDe(id);
	char j = turno(id);
	int k = 0;
	for (int i = 0; i < CELDAS; i++){
		if (t[i] == '.'){
			t[i] = j;
			suc[k] = idTablero(t);
			k = k + 1;
			t[i] = '.';
		}
	}
	return k;
}

// La busqueda del laboratorio 5 con una pila (retener y soltar alrededor del
// pop), que se detiene cuando esMeta es true. Entrega una partida valida,
// no necesariamente la mas corta.
LList* resolver(int idInicio){
	int n = cantidadIds();
	if (idInicio < 0 || idInicio >= n){
		return nullptr;
	}

	bool* visitado = new bool[n];
	for (int i = 0; i < n; i++){
		visitado[i] = false;
	}

	int suc[MAX_SUCESORES];
	Stack S;
	S.push(new DataNode(idInicio));
	DataNode* e = nullptr;
	bool encontrado = false;
	while (!S.isEmpty() && !encontrado){
		e = S.front()->getData();
		if (esMeta(e->getId())){
			encontrado = true;
		}
		else{
			visitado[e->getId()] = true;
			e->setNChildren(e->getNChildren() + 1);     // retener: el pop no la libera
			S.pop();
			int k = sucesores(e->getId(), suc);
			for (int i = 0; i < k; i++){
				if (!visitado[suc[i]]){
					S.push(new DataNode(suc[i], e));
				}
			}
			e->setNChildren(e->getNChildren() - 1);     // soltar
			if (e->getNChildren() == 0){
				delete e;
			}
		}
	}

	LList* camino = nullptr;
	if (encontrado){
		camino = new LList();
		DataNode* p = e;
		while (p != nullptr){
			camino->insert(p->getId());
			p = p->getFather();
		}
	}

	delete[] visitado;
	return camino;
}
