#include "poscode/poscode.hpp"

Poscode::Poscode() : data(""){

}

Poscode::Poscode(std::string _data) : data(_data){

}

char Poscode::getValue(size_t i){
	return data[i];
}

const std::string &Poscode::getData(){
	return data;
}
