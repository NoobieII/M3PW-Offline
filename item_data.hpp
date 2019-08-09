#ifndef ITEM_DATA_HPP
#define ITEM_DATA_HPP

#include <string>

struct ItemData{
	int id;
	bool is_stackable;
	char type;
	short mass;
	std::string name;
	std::string desc;
	
	//use item specific
	short hp;
	short mp;
}; 

#endif