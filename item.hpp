#ifndef ITEM_HPP
#define ITEM_HPP

class Item{
public:
	Item(int id, int amount, int unique_id_ = 0);
	~Item();
	
	int get_unique_id();
	int get_id();
	short get_amount();
	void set_amount(short amount);
	
private:
	unsigned int unique_id_;
	int id_;
	short amount_;
	
	//equip specific
};

#endif