#include "map.hpp"
#include "player.hpp"

Map::Map(int id):
	id_(id),
	players_()
{
}

Map::~Map(){
}

int Map::get_id(){
	return id_;
}

void Map::add_player(Player *p){
	players_[p->get_id()] = p;
	p->set_map(this);
}

void Map::remove_player(int id){
	players_[id]->set_map(0);
	players_.erase(id);
}

std::map<int, Player*> *Map::get_players(){
	return &players_;
}
