#include "busqueda/jarras.hpp"
#include "busqueda/stack.hpp"
#include "busqueda/queue.hpp"

// ===========================================================================
// 2a - El modelo
// ===========================================================================

// (a, b) -> a * 6 + b. Es como escribir el estado en base 6: b es la cifra de
// las unidades y a la de las "seisenas". Por eso se decodifica con / y %.
int idJarras(int a, int b){
	return a * (CAP_B + 1) + b;
}

int litrosA(int id){
	return id / (CAP_B + 1);
}

int litrosB(int id){
	return id % (CAP_B + 1);
}

static int minimo(int x, int y){
	if (x < y){
		return x;
	}
	return y;
}

// Cada operacion se agrega solo si cambia algo. Por ejemplo, llenar A no hace
// nada si A ya esta llena, y trasvasar de A a B no hace nada si A esta vacia
// o si B esta llena.
int sucesoresJarras(int id, int* suc){
	int a = litrosA(id);
	int b = litrosB(id);
	int k = 0;

	if (a < CAP_A){                                     // llenar A
		suc[k] = idJarras(CAP_A, b);
		k = k + 1;
	}
	if (b < CAP_B){                                     // llenar B
		suc[k] = idJarras(a, CAP_B);
		k = k + 1;
	}
	if (a > 0){                                         // vaciar A
		suc[k] = idJarras(0, b);
		k = k + 1;
	}
	if (b > 0){                                         // vaciar B
		suc[k] = idJarras(a, 0);
		k = k + 1;
	}
	if (a > 0 && b < CAP_B){                            // A -> B
		// Pasa lo que haya en A, pero no mas de lo que cabe en B.
		int t = minimo(a, CAP_B - b);
		suc[k] = idJarras(a - t, b + t);
		k = k + 1;
	}
	if (b > 0 && a < CAP_A){                            // B -> A
		int t = minimo(b, CAP_A - a);
		suc[k] = idJarras(a + t, b - t);
		k = k + 1;
	}
	return k;
}

// ===========================================================================
// 2b - Resolver con una pila
// ===========================================================================
//
// Es buscarCamino de la Parte 1 con tres cambios:
//   - el origen es el estado (0, 0);
//   - la meta es una CONDICION (b == meta), no un unico estado: sirven tanto
//     (0, 4) como (3, 4);
//   - la linea "para cada x en adj[e]" pasa a "para cada x en SUCESORES(e)".
//
// visitado puede ser un arreglo fijo, porque la cantidad de estados se conoce
// de antemano: N_JARRAS = 24.
// ---------------------------------------------------------------------------
LList* resolverJarras(int meta){
	bool visitado[N_JARRAS];
	for (int i = 0; i < N_JARRAS; i++){
		visitado[i] = false;
	}

	int suc[MAX_SUC_JARRAS];
	Stack S;
	S.push(new DataNode(idJarras(0, 0)));
	DataNode* e = nullptr;
	bool encontrado = false;
	while (!S.isEmpty() && !encontrado){
		e = S.front()->getData();
		if (litrosB(e->getId()) == meta){
			encontrado = true;
		}
		else{
			visitado[e->getId()] = true;
			e->setNChildren(e->getNChildren() + 1);
			S.pop();
			int k = sucesoresJarras(e->getId(), suc);
			for (int i = 0; i < k; i++){
				if (!visitado[suc[i]]){
					S.push(new DataNode(suc[i], e));
				}
			}
			e->setNChildren(e->getNChildren() - 1);
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
	return camino;
}

// ===========================================================================
// 2c - Resolver con una cola
// ===========================================================================
//
// Copia exacta de resolverJarras. Lo unico distinto es la linea marcada.
// ---------------------------------------------------------------------------
LList* resolverJarrasCola(int meta){
	bool visitado[N_JARRAS];
	for (int i = 0; i < N_JARRAS; i++){
		visitado[i] = false;
	}

	int suc[MAX_SUC_JARRAS];
	Queue S;                                            // <- la unica diferencia
	S.push(new DataNode(idJarras(0, 0)));
	DataNode* e = nullptr;
	bool encontrado = false;
	while (!S.isEmpty() && !encontrado){
		e = S.front()->getData();
		if (litrosB(e->getId()) == meta){
			encontrado = true;
		}
		else{
			visitado[e->getId()] = true;
			e->setNChildren(e->getNChildren() + 1);
			S.pop();
			int k = sucesoresJarras(e->getId(), suc);
			for (int i = 0; i < k; i++){
				if (!visitado[suc[i]]){
					S.push(new DataNode(suc[i], e));
				}
			}
			e->setNChildren(e->getNChildren() - 1);
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
	return camino;
}
