#ifndef BUSQUEDA_NODE_HPP
#define BUSQUEDA_NODE_HPP

/**
 * Nodo de una lista enlazada de enteros (libro, Cap. 4.1).
 *
 * Es el Node del codigo visto en clases: guarda un id y el puntero al
 * siguiente. Se usa para armar el mapa (las listas de vecinos) y para
 * entregar el camino encontrado.
 */
class Node{
	private:
		int id;
		Node* next;
	public:
		Node();
		Node(int id_, Node* next_ = nullptr);
		void setId(int id_);
		void setNext(Node* next_);
		int getId();
		Node* getNext();
		virtual ~Node();
};

#endif
