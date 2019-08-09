#include "constants.hpp"
#include "error_report.hpp"
#include "item_data.hpp"
#include "item_data_provider.hpp"
#include "pwfile.h"

#include <cstdio>
#include <cstring>

ItemDataProvider *ItemDataProvider::singleton_ = nullptr;

ItemDataProvider *ItemDataProvider::get_instance(){
	if(!singleton_){
		singleton_ = new ItemDataProvider;
	}
	return singleton_;
}

ItemDataProvider::ItemDataProvider():
	items_()
{
}

ItemDataProvider::~ItemDataProvider(){
}

int ItemDataProvider::load_data(){
	PWFILE *item_file = nullptr;
	PWRECORD *item_record = nullptr;
	ItemData *item_data = nullptr;
	char *item_name = (char*) malloc(32);
	char *item_desc = (char*) malloc(256);
	
	//load the item data
	item_file = PWF_load(ITEM_DATA_DIR);
	
	if(!item_file){
		log_error("ItemDataProvider::load_data() - could not load %s", ITEM_DATA_DIR);
		return -1;
	}
	
	item_record = PWF_begin(item_file);
	
	while(PWF_end(item_record) == 0){
		item_data = new ItemData;
		
		PWF_get(item_record,
			"id, is_stackable, type, mass, name, desc, hp, mp",
			&(item_data->id),
			&(item_data->is_stackable),
			&(item_data->type),
			&(item_data->mass),
			item_name,
			item_desc,
			&(item_data->hp),
			&(item_data->mp));
		
		
		item_data->name.resize(strlen(item_name) + 1);
		item_data->desc.resize(strlen(item_desc) + 1);
		memcpy(&(item_data->name[0]), item_name, strlen(item_name) + 1);
		memcpy(&(item_data->desc[0]), item_desc, strlen(item_desc) + 1);
		
		items_[item_data->id] = item_data;
		PWF_next(item_record);
	}
	
	PWF_free_record(item_record);
	PWF_close(item_file);
	
	free(item_name);
	free(item_desc);
	
	return 0;
}

void ItemDataProvider::free_data(){
	for(auto it : items_){
		delete it.second;
	}
	items_.clear();
}

std::map<int, ItemData*> *ItemDataProvider::get_item_data(){
	return &items_;
}

ItemData *ItemDataProvider::get_item_data_by_id(int id){
	if(items_.find(id) == items_.end()){
		log_error("ItemDataProvider::get_item_by_id() - no data found for id %d", id);
		return nullptr;
	}
	return items_[id];
}
