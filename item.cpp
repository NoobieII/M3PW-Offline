#include "item.hpp"

Item::Item(int id, int amount, int unique_id_):
	unique_id_(unique_id_),
	id_(id),
	amount_(amount)
{
}

Item::~Item(){
}

int Item::get_unique_id(){
	return unique_id_;
}

int Item::get_id(){
	return id_;
}

short Item::get_amount(){
	return amount_;
}

void Item::set_amount(short amount){
	amount_ = amount;
}
