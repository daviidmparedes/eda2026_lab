#ifndef TICTACTOE_DATANODE_HPP
#define TICTACTOE_DATANODE_HPP

/**
 * Ficha que se guarda en la pila. Es la misma del laboratorio 5.
 *
 *   id         el estado que representa
 *   father     la ficha padre (nullptr si no tiene)
 *   nchildren  cuantas fichas vivas tienen a esta como padre
 *
 * Memoria:
 *   - DataNode(id, father) le suma una hija a father;
 *   - ~DataNode le resta una hija a su padre y, si el padre queda con 0 hijas,
 *     libera tambien al padre.
 */
class DataNode{
	private:
		int id;
		DataNode* father;
		int nchildren;
	public:
		DataNode();
		DataNode(int id_);
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
