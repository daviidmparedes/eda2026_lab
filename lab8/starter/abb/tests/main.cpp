#include "abb/abb.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// Cada prueba marca [ok] o [FALLA]; cuando falla, indica el motivo.
//
// Para revisar, este archivo recorre los arboles por su cuenta, con los
// getters de ABBNode: revisa el orden de las llaves, el padre de cada nodo y
// cuantos nodos quedan en memoria. Nunca sigue un puntero a un nodo que ya se
// libero (ABBNode::estaVivo). Los arboles de las Partes 2 y 3 los arma el
// mismo, con setRoot, asi que esas partes no dependen de insert.

static int nOk = 0;
static int nFalla = 0;

static void check(const std::string &nombre, bool ok, const std::string &motivo){
	std::cout << "  " << std::left << std::setw(50) << nombre
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

static std::string num(int x){
	return std::to_string(x);
}

// Un nodo como texto, sin leer nunca un nodo que ya se libero.
static std::string txt(ABBNode *node){
	if (node == nullptr){
		return "nullptr";
	}
	if (!ABBNode::estaVivo(node)){
		return "un nodo ya liberado";
	}
	return num(node->getData());
}

static std::string lista(const std::vector<int> &v){
	std::string s = "{";
	for (int i = 0; i < (int)v.size(); i++){
		s = s + (i > 0 ? " " : "") + num(v[i]);
	}
	return s + "}";
}

static std::vector<int> sin(std::vector<int> v, int llave){
	v.erase(std::find(v.begin(), v.end(), llave));
	return v;
}

/* El arbol de la Figura 6.9 del libro, con tres llaves mas (1, 12 y 19)
   para que aparezcan todos los casos de remove:

                  16
              /        \
             4          30
           /   \      /    \
          2    10    18     50
         /       \     \    /
        1        12    19  34
*/
static const std::vector<int> FIGURA = {16, 4, 30, 2, 10, 18, 50, 34, 1, 12, 19};

// ---------------------------------------------------------------------------
// Referencias propias del test
// ---------------------------------------------------------------------------

// Arma un ABB con las llaves en ese orden, sin usar insert.
static void construir(ABB &abb, const std::vector<int> &llaves){
	ABBNode *raiz = nullptr;
	for (int i = 0; i < (int)llaves.size(); i++){
		int k = llaves[i];
		if (raiz == nullptr){
			raiz = new ABBNode(k);
		}
		else{
			ABBNode *p = raiz;
			bool listo = false;
			while (!listo){
				if (k < p->getData()){
					if (p->getLeft() == nullptr){
						p->setLeft(new ABBNode(k, p));
						listo = true;
					}
					else{
						p = p->getLeft();
					}
				}
				else{
					if (p->getRight() == nullptr){
						p->setRight(new ABBNode(k, p));
						listo = true;
					}
					else{
						p = p->getRight();
					}
				}
			}
		}
	}
	abb.setRoot(raiz);
}

static ABBNode *buscar(ABBNode *p, int k){
	while (p != nullptr && p->getData() != k){
		p = (k < p->getData()) ? p->getLeft() : p->getRight();
	}
	return p;
}

// Un tope de nodos para no recorrer para siempre un arbol con un ciclo.
static const int TOPE = 100000;

// La forma del arbol como texto: 16(4(2(1,-),10(-,12)),...), con - para un
// hijo que no existe. Una hoja es solo su llave.
static std::string forma(ABBNode *node, int &visitados){
	if (node == nullptr){
		return "-";
	}
	if (!ABBNode::estaVivo(node)){
		return "?";
	}
	visitados++;
	if (visitados > TOPE){
		return "...";
	}
	std::string s = num(node->getData());
	if (node->getLeft() != nullptr || node->getRight() != nullptr){
		s = s + "(" + forma(node->getLeft(), visitados) + "," + forma(node->getRight(), visitados) + ")";
	}
	return s;
}

static std::string forma(ABBNode *node){
	int visitados = 0;
	return forma(node, visitados);
}

static int altura(ABBNode *node){
	if (node == nullptr){
		return -1;
	}
	return 1 + std::max(altura(node->getLeft()), altura(node->getRight()));
}

// Recorre el subarbol de node revisando que este vivo, que su padre sea padre
// y que su llave quede bien ordenada respecto de sus ancestros: menor que la
// de menorQue y mayor que la de mayorQue. Deja las llaves en in-orden.
static bool revisarNodo(ABBNode *node, ABBNode *padre, const std::string &donde,
                        ABBNode *menorQue, ABBNode *mayorQue,
                        std::vector<int> &llaves, int &visitados, std::string &motivo){
	if (node == nullptr){
		return true;
	}
	if (!ABBNode::estaVivo(node)){
		motivo = donde + " apunta a un nodo que ya se libero";
		return false;
	}
	visitados++;
	if (visitados > TOPE){
		motivo = "el arbol tiene un ciclo: el recorrido no termina";
		return false;
	}
	if (node->getParent() != padre){
		motivo = "el padre de " + txt(node) + " es " + txt(node->getParent()) + " y deberia ser " + txt(padre);
		return false;
	}
	if (menorQue != nullptr && !(node->getData() < menorQue->getData())){
		motivo = txt(node) + " esta en el subarbol izquierdo de " + txt(menorQue) + " y no es menor";
		return false;
	}
	if (mayorQue != nullptr && !(node->getData() > mayorQue->getData())){
		motivo = txt(node) + " esta en el subarbol derecho de " + txt(mayorQue) + " y no es mayor";
		return false;
	}
	if (!revisarNodo(node->getLeft(), node, "el hijo izquierdo de " + txt(node),
	                 node, mayorQue, llaves, visitados, motivo)){
		return false;
	}
	llaves.push_back(node->getData());
	return revisarNodo(node->getRight(), node, "el hijo derecho de " + txt(node),
	                   menorQue, node, llaves, visitados, motivo);
}

// Revisa que abb sea un ABB bien armado, con exactamente las llaves
// esperadas, y que en memoria no sobren ni falten nodos. base es cuantos
// nodos habia vivos antes de armar el arbol.
static bool revisar(ABB &abb, std::vector<int> esperadas, int base, std::string &motivo){
	std::sort(esperadas.begin(), esperadas.end());
	std::vector<int> llaves;
	int visitados = 0;
	if (!revisarNodo(abb.getRoot(), nullptr, "la raiz del arbol", nullptr, nullptr, llaves, visitados, motivo)){
		return false;
	}
	int i = 0;
	while (i < (int)llaves.size() && i < (int)esperadas.size() && llaves[i] == esperadas[i]){
		i++;
	}
	if (i < (int)esperadas.size() && (i >= (int)llaves.size() || esperadas[i] < llaves[i])){
		motivo = "falta la llave " + num(esperadas[i]);
		return false;
	}
	if (i < (int)llaves.size()){
		motivo = "la llave " + num(llaves[i]) + " esta en el arbol y no deberia";
		return false;
	}
	int enMemoria = ABBNode::nodosVivos() - base;
	int n = esperadas.size();
	if (enMemoria > n){
		motivo = "hay " + num(enMemoria) + " nodos en memoria y el arbol tiene " + num(n) + ": falta un delete";
		return false;
	}
	if (enMemoria < n){
		motivo = "hay " + num(enMemoria) + " nodos en memoria y el arbol tiene " + num(n) + ": se libero un nodo de mas";
		return false;
	}
	return true;
}

// ===========================================================================
// Parte 1 - Insertar, buscar y recorrer
// ===========================================================================

static void testInsertFigura(){
	int base = ABBNode::nodosVivos();
	ABB abb;
	bool ok = true;
	std::string motivo;
	for (int i = 0; ok && i < (int)FIGURA.size(); i++){
		if (!abb.insert(FIGURA[i])){
			ok = false;
			motivo = "insert(" + num(FIGURA[i]) + ") retorna false, y la llave no estaba";
		}
	}
	if (ok){
		std::string esperada;
		{
			ABB ref;
			construir(ref, FIGURA);
			esperada = forma(ref.getRoot());
		}
		std::string obtenida = forma(abb.getRoot());
		if (obtenida != esperada){
			ok = false;
			motivo = "se esperaba la forma " + esperada + " y se obtuvo " + obtenida;
		}
	}
	if (ok){
		ok = revisar(abb, FIGURA, base, motivo);
	}
	check("insert arma el arbol de la figura", ok, motivo);
	if (ok){
		abb.imprimir();
	}
}

static void testInsertRepetida(){
	int base = ABBNode::nodosVivos();
	ABB abb;
	bool a = abb.insert(5);
	bool b = abb.insert(3);
	bool c = abb.insert(5);
	bool d = abb.insert(3);
	bool ok = false;
	std::string motivo;
	if (!a || !b){
		motivo = "insert retorna false con una llave nueva";
	}
	else if (c || d){
		motivo = "insert(" + num(c ? 5 : 3) + ") retorna true, y la llave ya estaba";
	}
	else{
		ok = revisar(abb, {5, 3}, base, motivo);
	}
	check("insert no repite llaves", ok, motivo);
}

static bool testFindPresentes(){
	ABB abb;
	construir(abb, FIGURA);
	bool ok = true;
	std::string motivo;
	for (int i = 0; ok && i < (int)FIGURA.size(); i++){
		int k = FIGURA[i];
		ABBNode *n = abb.find(k);
		if (n != buscar(abb.getRoot(), k)){
			ok = false;
			motivo = "find(" + num(k) + ") retorna " + (n == nullptr ? "nullptr" : "el nodo de " + txt(n));
		}
	}
	check("find encuentra las 11 llaves", ok, motivo);
	return ok;
}

// Las llaves que no estan terminan en hijos que no existen, a distintas
// alturas del arbol.
static void testFindAusentes(bool presentesOk){
	const std::string nombre = "find retorna nullptr si la llave no esta";
	if (!presentesOk){
		check(nombre, false, "primero find tiene que encontrar las llaves que si estan");
		return;
	}
	ABB abb;
	construir(abb, FIGURA);
	const int AUSENTES[6] = {0, 3, 11, 17, 31, 99};
	bool ok = true;
	std::string motivo;
	for (int i = 0; ok && i < 6; i++){
		ABBNode *n = abb.find(AUSENTES[i]);
		if (n != nullptr){
			ok = false;
			motivo = "find(" + num(AUSENTES[i]) + ") retorna el nodo de " + txt(n) + ", y la llave no esta";
		}
	}
	if (ok){
		ABB vacio;
		if (vacio.find(16) != nullptr){
			ok = false;
			motivo = "en un arbol vacio, find(16) no retorna nullptr";
		}
	}
	check(nombre, ok, motivo);
}

static void testInOrder(){
	ABB abb;
	construir(abb, FIGURA);
	const int GUARDA = -123456;
	const int N = FIGURA.size();
	std::vector<int> keys(N + 4, GUARDA);
	int n = abb.inOrder(keys.data());
	std::vector<int> esperadas = FIGURA;
	std::sort(esperadas.begin(), esperadas.end());
	bool ok = true;
	std::string motivo;
	if (n != N){
		ok = false;
		motivo = "inOrder retorna " + num(n) + " y el arbol tiene " + num(N) + " llaves";
	}
	for (int i = N; ok && i < N + 4; i++){
		if (keys[i] != GUARDA){
			ok = false;
			motivo = "inOrder escribe mas alla de las " + num(N) + " llaves";
		}
	}
	if (ok){
		std::vector<int> obtenidas(keys.begin(), keys.begin() + N);
		if (obtenidas != esperadas){
			ok = false;
			motivo = "se esperaba " + lista(esperadas) + " y se obtuvo " + lista(obtenidas);
		}
	}
	if (ok){
		ABB vacio;
		int k2[4];
		int m = vacio.inOrder(k2);
		if (m != 0){
			ok = false;
			motivo = "en un arbol vacio, inOrder retorna " + num(m);
		}
	}
	check("inOrder entrega las llaves de menor a mayor", ok, motivo);
}

// ===========================================================================
// Parte 2 - El menor y el mayor
// ===========================================================================

struct Extremo{
	int desde;          // la llave del nodo donde parte el subarbol
	bool menor;         // true: getLowest; false: getHighest
	int esperado;
};

static void testExtremos(const std::string &nombre, const Extremo *casos, int n){
	ABB abb;
	construir(abb, FIGURA);
	bool ok = true;
	std::string motivo;
	for (int i = 0; ok && i < n; i++){
		ABBNode *sub = buscar(abb.getRoot(), casos[i].desde);
		ABBNode *r = casos[i].menor ? abb.getLowest(sub) : abb.getHighest(sub);
		if (r != buscar(abb.getRoot(), casos[i].esperado)){
			ok = false;
			motivo = std::string(casos[i].menor ? "getLowest" : "getHighest") + "(nodo " + num(casos[i].desde)
			       + ") retorna " + txt(r) + " y deberia retornar el nodo " + num(casos[i].esperado);
		}
	}
	check(nombre, ok, motivo);
}

static void testExtremosRaiz(){
	const Extremo casos[2] = {{16, true, 1}, {16, false, 50}};
	testExtremos("getLowest y getHighest de la raiz", casos, 2);
}

static void testExtremosSubarboles(){
	const Extremo casos[6] = {
		{30, true, 18}, {4, false, 12},     // bajando varios niveles
		{10, true, 10}, {50, false, 50},    // el nodo mismo: no tiene hijo de ese lado
		{34, true, 34}, {2, false, 2}
	};
	testExtremos("getLowest y getHighest de seis subarboles", casos, 6);
}

// ===========================================================================
// Parte 3 - Eliminar
// ===========================================================================

struct Caso{
	int llave;
	const char *nombre;
	bool dibujar;
};

static const int N_CASOS = 9;
static const Caso CASOS[N_CASOS] = {
	{1,  "una hoja izquierda: 1", false},
	{12, "una hoja derecha: 12", false},
	{2,  "un hijo: 2, izquierdo, con hijo izquierdo", false},
	{10, "un hijo: 10, derecho, con hijo derecho", false},
	{18, "un hijo: 18, izquierdo, con hijo derecho", false},
	{50, "un hijo: 50, derecho, con hijo izquierdo", false},
	{4,  "dos hijos: 4", true},
	{30, "dos hijos: 30", true},
	{16, "dos hijos: 16, la raiz", true}
};

static void testCasos(){
	for (int i = 0; i < N_CASOS; i++){
		int base = ABBNode::nodosVivos();
		ABB abb;
		construir(abb, FIGURA);
		abb.remove(buscar(abb.getRoot(), CASOS[i].llave));
		std::string motivo;
		bool ok = revisar(abb, sin(FIGURA, CASOS[i].llave), base, motivo);
		check(CASOS[i].nombre, ok, motivo);
		if (ok && CASOS[i].dibujar){
			abb.imprimir();
		}
	}
}

static void testRaizUnHijo(){
	int base = ABBNode::nodosVivos();
	bool ok = true;
	std::string motivo;
	{
		ABB abb;
		construir(abb, {10, 20, 30});
		abb.remove(abb.getRoot());
		ok = revisar(abb, {20, 30}, base, motivo);
		if (!ok){
			motivo = "la raiz 10 con su hijo derecho 20: " + motivo;
		}
	}
	if (ok){
		ABB abb;
		construir(abb, {30, 20, 10});
		abb.remove(abb.getRoot());
		ok = revisar(abb, {20, 10}, base, motivo);
		if (!ok){
			motivo = "la raiz 30 con su hijo izquierdo 20: " + motivo;
		}
	}
	check("la raiz, con un solo hijo", ok, motivo);
}

static void testUnicoNodo(){
	int base = ABBNode::nodosVivos();
	ABB abb;
	construir(abb, {7});
	abb.remove(abb.getRoot());
	std::string motivo;
	bool ok = revisar(abb, {}, base, motivo);
	check("el unico nodo: el arbol queda vacio", ok, motivo);
}

// ===========================================================================
// Parte 4 - Todo junto
// ===========================================================================

// Saca las llaves de orden una por una, con find y remove, y revisa el arbol
// despues de cada una. quedan son las llaves que tiene el arbol al empezar.
static bool eliminarTodas(ABB &abb, const std::vector<int> &orden, std::vector<int> quedan,
                          int base, std::string &motivo){
	for (int i = 0; i < (int)orden.size(); i++){
		int k = orden[i];
		ABBNode *n = abb.find(k);
		if (n == nullptr){
			motivo = "find(" + num(k) + ") no encuentra la llave";
			return false;
		}
		abb.remove(n);
		quedan = sin(quedan, k);
		if (!revisar(abb, quedan, base, motivo)){
			motivo = "despues de eliminar " + num(k) + " (eliminacion " + num(i + 1) + " de "
			       + num(orden.size()) + "): " + motivo;
			return false;
		}
	}
	return true;
}

static void testSecuencia(){
	int base = ABBNode::nodosVivos();
	ABB abb;
	bool ok = true;
	std::string motivo;
	for (int i = 0; ok && i < (int)FIGURA.size(); i++){
		if (!abb.insert(FIGURA[i])){
			ok = false;
			motivo = "insert(" + num(FIGURA[i]) + ") retorna false: revisa la Parte 1";
		}
	}
	if (ok){
		abb.remove(nullptr);
		ok = revisar(abb, FIGURA, base, motivo);
		if (!ok){
			motivo = "despues de remove(nullptr): " + motivo;
		}
	}
	if (ok){
		const std::vector<int> ORDEN = {16, 30, 4, 50, 2, 19, 10, 1, 34, 12, 18};
		ok = eliminarTodas(abb, ORDEN, FIGURA, base, motivo);
	}
	check("las 11 llaves de la figura, una por una", ok, motivo);
}

// Un generador propio, para que las llaves sean las mismas en todo computador.
static unsigned int semilla = 2026;

static int azar(int n){
	semilla = semilla * 1103515245u + 12345u;
	return (int)((semilla / 65536u) % (unsigned int)n);
}

static void barajar(std::vector<int> &v){
	for (int i = (int)v.size() - 1; i > 0; i--){
		std::swap(v[i], v[azar(i + 1)]);
	}
}

static void testAzar(){
	const int N = 1000;
	int base = ABBNode::nodosVivos();
	std::vector<int> todas;
	for (int i = 0; i < 3 * N; i++){
		todas.push_back(i);
	}
	barajar(todas);
	std::vector<int> llaves(todas.begin(), todas.begin() + N);

	ABB abb;
	bool ok = true;
	std::string motivo;
	for (int i = 0; ok && i < N; i++){
		if (!abb.insert(llaves[i])){
			ok = false;
			motivo = "insert(" + num(llaves[i]) + ") retorna false con una llave nueva";
		}
	}
	if (ok){
		ok = revisar(abb, llaves, base, motivo);
		if (!ok){
			motivo = "despues de insertar las " + num(N) + " llaves: " + motivo;
		}
	}
	int h = ok ? altura(abb.getRoot()) : -1;
	if (ok){
		std::vector<int> orden = llaves;
		barajar(orden);
		ok = eliminarTodas(abb, orden, llaves, base, motivo);
	}
	check(num(N) + " llaves al azar: insertar y eliminar todas", ok, motivo);
	if (ok){
		std::cout << "      altura con " << N << " llaves: " << h
		          << " (un arbol balanceado tendria 9)" << std::endl;
	}
}

// ---------------------------------------------------------------------------

int main(int nargs, char **vargs){
	std::cout << "Laboratorio 8 - Arbol binario de busqueda" << std::endl;

	parte("Parte 1 - Insertar, buscar y recorrer");
	testInsertFigura();
	testInsertRepetida();
	bool presentesOk = testFindPresentes();
	testFindAusentes(presentesOk);
	testInOrder();

	parte("Parte 2 - El menor y el mayor");
	testExtremosRaiz();
	testExtremosSubarboles();

	parte("Parte 3 - Eliminar");
	testCasos();
	testRaizUnHijo();
	testUnicoNodo();

	parte("Parte 4 - Todo junto");
	testSecuencia();
	testAzar();

	std::cout << std::endl << "Resumen: " << nOk << " de " << (nOk + nFalla)
	          << " correctas" << std::endl;
	return 0;
}
