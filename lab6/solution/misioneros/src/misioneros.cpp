#include "misioneros/misioneros.hpp"
#include "misioneros/stack.hpp"
#include "misioneros/queue.hpp"

// ===========================================================================
// SOLUCION DE REFERENCIA
// ===========================================================================

// (m, c, b) se escribe como un numero en base mixta: b es la cifra de las
// unidades (2 valores), c la siguiente (CANIBALES + 1 valores) y m la mas
// significativa (MISIONEROS + 1 valores). Con 3 y 3 da ids de 0 a 31.
int idEstado(int m, int c, int b){
	return (m * (CANIBALES + 1) + c) * 2 + b;
}

void estadoDe(int id, int &m, int &c, int &b){
	b = id % 2;
	c = (id / 2) % (CANIBALES + 1);
	m = (id / 2) / (CANIBALES + 1);
}

int cantidadIds(){
	return (MISIONEROS + 1) * (CANIBALES + 1) * 2;
}

bool valido(int m, int c){
	if (m < 0 || m > MISIONEROS || c < 0 || c > CANIBALES){
		return false;
	}
	// Orilla izquierda.
	if (m > 0 && c > m){
		return false;
	}
	// Orilla derecha.
	int md = MISIONEROS - m;
	int cd = CANIBALES - c;
	if (md > 0 && cd > md){
		return false;
	}
	return true;
}

bool esMeta(int id){
	return id == idEstado(0, 0, 0);
}

// Los cinco cruces posibles con un bote de 2, en el orden de la convencion
// del control: 1 misionero, 2 misioneros, 1 canibal, 2 canibales, 1 y 1.
// Si el bote esta a la izquierda las personas se restan de (m, c); si esta a
// la derecha, se suman. El rango que revisa valido() cubre el caso en que en
// la orilla del bote no hay suficientes personas para el cruce.
int sucesores(int id, int* suc){
	int m, c, b;
	estadoDe(id, m, c, b);
	const int dm[MAX_SUCESORES] = {1, 2, 0, 0, 1};
	const int dc[MAX_SUCESORES] = {0, 0, 1, 2, 1};
	int k = 0;
	for (int i = 0; i < MAX_SUCESORES; i++){
		int nm = 0;
		int nc = 0;
		int nb = 0;
		if (b == 1){
			nm = m - dm[i];
			nc = c - dc[i];
			nb = 0;
		}
		else{
			nm = m + dm[i];
			nc = c + dc[i];
			nb = 1;
		}
		if (valido(nm, nc)){
			suc[k] = idEstado(nm, nc, nb);
			k = k + 1;
		}
	}
	return k;
}

// La busqueda del laboratorio 5, con una cola: entrega la secuencia con menos
// cruces. Con una pila tambien pasa todas las pruebas (encuentra una
// secuencia valida, no necesariamente la mas corta).
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
	Queue S;
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
