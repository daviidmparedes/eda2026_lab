#include "busqueda/datanode.hpp"

DataNode::DataNode(): id(-1), father(nullptr), nchildren(0){
}

DataNode::DataNode(int id_): id(id_), father(nullptr), nchildren(0){
}

DataNode::DataNode(int id_, DataNode* father_): id(id_), father(father_), nchildren(0){
	if (father != nullptr){
		father->setNChildren(father->getNChildren() + 1);
	}
}

void DataNode::setId(int id_){
	id = id_;
}

void DataNode::setFather(DataNode* father_){
	father = father_;
}

void DataNode::setNChildren(int n){
	nchildren = n;
}

int DataNode::getId(){
	return id;
}

DataNode* DataNode::getFather(){
	return father;
}

int DataNode::getNChildren(){
	return nchildren;
}

// La ficha deja de existir: su padre tiene una hija menos. Si esa era la
// ultima, nadie mas necesita al padre y se libera tambien (y el padre, al
// liberarse, hace lo mismo con el suyo).
DataNode::~DataNode(){
	if (father != nullptr){
		father->setNChildren(father->getNChildren() - 1);
		if (father->getNChildren() == 0){
			delete father;
		}
	}
}
