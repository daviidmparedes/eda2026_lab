#include "tictactoe/stacknode.hpp"

StackNode::StackNode(): data(nullptr), next(nullptr){
}

StackNode::StackNode(DataNode* data_): data(data_), next(nullptr){
}

void StackNode::setData(DataNode* data_){
	data = data_;
}

void StackNode::setNext(StackNode* next_){
	next = next_;
}

DataNode* StackNode::getData(){
	return data;
}

StackNode* StackNode::getNext(){
	return next;
}

// Se bota la caja. La ficha se libera con ella solo si no tiene hijas.
StackNode::~StackNode(){
	if (data != nullptr && data->getNChildren() == 0){
		delete data;
	}
}
