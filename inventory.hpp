#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "item.hpp"

#include <list>
#include <memory> //for std::shared_ptr

class Inventory{
public:
	Inventory(int weight_limit);
	
	bool is_full();
	
	//k should be equal to 1, with normal conditions. A lower k means the items weigh less.
	void set_weight_coefficient(int k);
	float get_weight_coefficient();
	int get_weight();
	void set_weight_limit(int weight_limit);
	int get_weight_limit();
	int get_num_items();
	
	void recalculate_weight();
	
	//adding/giving items will recalculate the mass.
	//add existing item instance to the inventory.
	//Returns 0 when successful, -1 if not successful
	int add_item(std::shared_ptr<Item> item);
	//make a new item instance and add it to inventory
	void give_item(int id, int amount);
	
	std::shared_ptr<Item> get_item(int id);
	std::shared_ptr<Item> get_item_by_slot(int slot);
	
	//removes item from the inventory.
	//To remove all items of the id/slot, set amount to -1.
	//mass will be recalculated when items are removed.
	void remove_item(int id, int amount);
	void remove_item_by_slot(int slot, int amount);
private:
	//scaling factor for the weight of all carried items. 1.0 is the normal value (on earth).
	float weight_coefficient_;
	//mass is kept on a variable for quick access without recalculation.
	int mass_;
	int weight_limit_;
	std::list<std::shared_ptr<Item> > items_;
};

#endif