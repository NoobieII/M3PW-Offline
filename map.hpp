#ifndef MAP_HPP
#define MAP_HPP

#include <map>

class MapData;
class Player;

class Map{
public:
	Map(int id);
	Map(MapData *map_data);
	~Map();
	
	int get_id();
	void add_player(Player *p);
	void remove_player(int id);
	std::map<int, Player*> *get_players();
private:
	Map(const Map &m);
	int id_;
	std::map<int, Player*> players_;
};

#endif