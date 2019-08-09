#ifndef ITEM_DATA_PROVIDER_HPP
#define ITEM_DATA_PROVIDER_HPP

#include <map>

class ItemData;

class ItemDataProvider{
public:
	static ItemDataProvider *get_instance();
	int load_data();
	void free_data();
	
	std::map<int, ItemData*> *get_item_data();
	ItemData *get_item_data_by_id(int id);
private:
	ItemDataProvider();
	~ItemDataProvider();
	static ItemDataProvider *singleton_;
	
	std::map<int, ItemData*> items_;
};

#endif