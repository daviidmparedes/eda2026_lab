#include "busqueda/jarras.hpp"
#include "busqueda/stack.hpp"
#include "busqueda/queue.hpp"

// ===========================================================================
// AQUI TRABAJAS TU.
//
// Parte 2: primero el modelo (2a), despues la busqueda (2b y 2c).
// Lee include/busqueda/jarras.hpp antes de empezar: ahi estan las 4 preguntas
// ya respondidas.
// ===========================================================================

// ---------------------------------------------------------------------------
// 2a - El modelo
// ---------------------------------------------------------------------------

// El estado (a, b) se guarda como un solo entero. B tiene CAP_B + 1 = 6
// valores posibles (0 a 5 litros), asi que id = a * 6 + b.
// Ejemplo para revisar a mano: (2, 5) -> 2 * 6 + 5 = 17.
//
// Usa las constantes CAP_A y CAP_B, no los numeros 3 y 5 escritos a mano.
int idJarras(int a, int b){
	// TODO: implementar.
	return 0;
}

// De vuelta: a partir del id, cuantos litros hay en A y cuantos en B.
// Pista: son la division y el resto de dividir por CAP_B + 1.
int litrosA(int id){
	// TODO: implementar.
	return 0;
}

int litrosB(int id){
	// TODO: implementar.
	return 0;
}

// Parte con:
//
//     int a = litrosA(id);
//     int b = litrosB(id);
//     int k = 0;
//
// y revisa las 6 operaciones EN ESTE ORDEN:
//
//     llenar A     llenar B     vaciar A     vaciar B     A -> B     B -> A
//
// Para cada una: si cambia algo, calcula el estado nuevo (na, nb), escribe
// idJarras(na, nb) en suc[k] y suma 1 a k. Al final retorna k.
//
// Cuando una operacion no cambia nada: llenar una jarra que ya esta llena,
// vaciar una que ya esta vacia, trasvasar desde una vacia o hacia una llena.
//
// Trasvasar de A a B: pasa lo que haya en A, pero no mas de lo que cabe en B,
// que es CAP_B - b. O sea, pasan t litros, con t el menor entre a y CAP_B - b.
//
// Ejemplo para revisar a mano: los sucesores de (2, 5) son, en orden,
// (3, 5) (0, 5) (2, 0) (3, 4). Llenar B y A -> B no cambian nada, porque B ya
// esta llena.
int sucesoresJarras(int id, int* suc){
	// TODO: implementar.
	return 0;
}

// ---------------------------------------------------------------------------
// 2b - Resolver con una pila
// ---------------------------------------------------------------------------
//
// Copia tu buscarCamino de la Parte 1 y cambia solo esto:
//
//   - visitado: la cantidad de estados se conoce de antemano, asi que basta
//     un arreglo fijo, bool visitado[N_JARRAS], sin new ni delete.
//   - el origen es idJarras(0, 0).
//   - la meta es una CONDICION: litrosB(e->getId()) == meta. Hay dos estados
//     que la cumplen para meta = 4: (0, 4) y (3, 4).
//   - la linea de los vecinos: en vez de recorrer adj[e], se calculan.
//
//         int suc[MAX_SUC_JARRAS];              // declaralo antes del while
//         ...
//         int k = sucesoresJarras(e->getId(), suc);
//         for (int i = 0; i < k; i++){
//             si !visitado[suc[i]], apilar new DataNode(suc[i], e)
//         }
//
// Todo lo demas, incluido el retener / soltar alrededor del pop y el armado
// del camino, queda exactamente igual.
// ---------------------------------------------------------------------------
LList* resolverJarras(int meta){
	// TODO: implementar.
	return nullptr;
}

// ---------------------------------------------------------------------------
// 2c - Resolver con una cola
// ---------------------------------------------------------------------------
//
// Copia resolverJarras y cambia Stack por Queue. Compara cuantos pasos da
// cada una.
// ---------------------------------------------------------------------------
LList* resolverJarrasCola(int meta){
	// TODO: implementar.
	return nullptr;
}
