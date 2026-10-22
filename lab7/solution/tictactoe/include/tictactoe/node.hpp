#ifndef TICTACTOE_NODE_HPP
#define TICTACTOE_NODE_HPP

/**
 * Nodo de una lista enlazada de enteros (libro, Cap. 4.1): un id y el puntero
 * al siguiente. Es el mismo del laboratorio 5.
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
