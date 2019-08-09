#include "error_report.hpp"
#include "inventory.hpp"
#include "item.hpp"
#include "item_data.hpp"
#include "item_data_provider.hpp"

Inventory::Inventory(int weight_limit):
	weight_coefficient_(1.0),
	mass_(0),
	weight_limit_(weight_limit),
	items_()
{
}


bool Inventory::is_full(){
	return ((int)(mass_ * weight_coefficient_)) >= weight_limit_;
}

//k should be equal to 1, with normal conditions. A lower k means the items weigh less.
void Inventory::set_weight_coefficient(int k){
	weight_coefficient_ = k;
}

float Inventory::get_weight_coefficient(){
	return weight_coefficient_;
}

int Inventory::get_weight(){
	return ((int)(mass_ * weight_coefficient_));
}

void Inventory::set_weight_limit(int weight_limit){
	weight_limit_ = weight_limit;
}

int Inventory::get_weight_limit(){
	return weight_limit_;
}

int Inventory::get_num_items(){
	return items_.size();
}

void Inventory::recalculate_weight(){
	mass_ = 0;
	
	for(auto &it : items_){
		ItemData *item_data = ItemDataProvider::get_instance()->get_item_data_by_id(it->get_id());
		mass_ += it->get_amount() * item_data->mass;
	}
}

//add existing item instance to the inventory
int Inventory::add_item(std::shared_ptr<Item> item){
	ItemData *item_data = nullptr;
	
	if(!item){
		log_error("Inventory::add_item() - null item given in parameter");
		return -1;
	}
	
	item_data = ItemDataProvider::get_instance()->get_item_data_by_id(item->get_id());
	if(!item_data){
		return -1;
	}
	
	//check if the item is not stackable.
	if(item_data->is_stackable == 0){
		items_.push_back(item);
		recalculate_weight();
		return 0;
	}
	
	//check if the item with the same id is already in the inventory
	for(auto &it : items_){
		if(item->get_id() == it->get_id()){
			item->set_amount(item->get_amount() + it->get_amount());
			it = item;
			recalculate_weight();
			return 0;
		}
	}
	
	//the item doesn't exist in the inventory
	items_.push_back(item);
	recalculate_weight();
	return 0;
}

//make a new item instance and add it to inventory
void Inventory::give_item(int id, int amount){
	std::shared_ptr<Item> item(new Item(id, amount));
	
	add_item(item);
}

std::shared_ptr<Item> Inventory::get_item(int id){
	for(auto &it : items_){
		if(it->get_id() == id){
			return it;
		}
	}
	return std::shared_ptr<Item>(nullptr);
}

//TODO: find correct function to return shared ptr, not the iterator
std::shared_ptr<Item> Inventory::get_item_by_slot(int slot){
	auto it = items_.begin();
	
	for(int i = 0; i < slot && it != items_.end(); ++i, it++);
	
	if(it == items_.end()){
		return std::shared_ptr<Item>(nullptr);
	}
	return *it;
}

void Inventory::remove_item(int id, int amount){
	//int amount_removed = 0;
	
	for(auto it = items_.begin(); it != items_.end();){
		if((*it)->get_id() == id){
			if(amount == -1){
				items_.erase(it++);
			}
			else if((*it)->get_amount() > amount){
				(*it)->set_amount((*it)->get_amount() - amount);
				it++;
			}
			else{
				items_.erase(it++);
			}
		}
		else{
			it++;
		}
	}
	
	recalculate_weight();
}
	
void Inventory::remove_item_by_slot(int slot, int amount){
	int i = 0;
	auto it = items_.begin();
	while(it != items_.end()){
		if(i == slot){
			if(amount == -1){
				items_.erase(it++);
			}
			else if((*it)->get_amount() > amount){
				(*it)->set_amount((*it)->get_amount() - amount);
				it++;
			}
			else{
				items_.erase(it++);
			}
		}
		else{
			it++;
		}
		i++;
	}
	
	recalculate_weight();
}
