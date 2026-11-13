#include "puzzle/estado.hpp"

// ===========================================================================
// Lo que viene dado de la clase Estado. Este archivo no se modifica.
// ===========================================================================

Estado::Estado(const std::string& tablero_, Estado* padre_, char accion_):
	tablero(tablero_), padre(padre_), accion(accion_){
}

const std::string& Estado::getTablero(){
	return tablero;
}
