#ifndef BUSQUEDA_DATANODE_HPP
#define BUSQUEDA_DATANODE_HPP

/**
 * La ficha que viaja por la pila (o por la cola) durante una busqueda.
 *
 *   id         el estado que representa: un vertice, un par de jarras o una
 *              celda del laberinto, siempre convertido a un entero
 *   father     la ficha desde la que se llego a esta. Es la HISTORIA: siguiendo
 *              los father desde la meta se recupera el camino (al reves)
 *   nchildren  cuantas fichas vivas tienen a esta como padre
 *
 * Las fichas que salen de la pila no se botan mientras alguna hija las
 * necesite: juntas forman un arbol, y el camino es una rama de ese arbol.
 *
 * Gestion de memoria (ya resuelta, pero hay que entenderla):
 *   - cuando una caja sale de la pila, su ficha se libera solo si no tiene
 *     hijas (ver ~StackNode);
 *   - al liberarse, una ficha le descuenta una hija a su padre, y si el padre
 *     queda sin hijas tambien se libera (ver ~DataNode). La memoria se va
 *     devolviendo en cascada, rama por rama.
 */
class DataNode{
	private:
		int id;
		DataNode* father;
		int nchildren;
	public:
		DataNode();
		DataNode(int id_);
		// Ficha hija de father_: la anota como padre y le suma una hija.
		// Es el ficha(x, padre = e) del pseudocodigo.
		DataNode(int id_, DataNode* father_);
		void setId(int id_);
		void setFather(DataNode* father_);
		void setNChildren(int n);
		int getId();
		DataNode* getFather();
		int getNChildren();
		~DataNode();
};

#endif
