#ifndef PLAYER_HPP
#define PLAYER_HPP

class Inventory;
class Map;

class Player{
public:
	Player();
	~Player();
	
	int get_id();
	void set_stance(int stance);
	int get_stance();
	void set_map(Map *m);
	Map *get_map();
	void set_x_position(int x);
	int get_x_position();
	void set_y_position(int y);
	int get_y_position();
	void set_hp(int hp);
	int get_hp();
	void set_max_hp(int max_hp);
	int get_max_hp();
	void set_mp(int mp);
	int get_mp();
	void set_max_mp(int max_mp);
	int get_max_mp();
	void set_exp(int exp);
	int get_exp();
	void set_max_exp(int max_exp);
	int get_max_exp();
	Inventory *get_inventory();
private:
	Player(const Player &p);
	
	int id_;
	int stance_;
	int x_;
	int y_;
	int hp_;
	int max_hp_;
	int mp_;
	int max_mp_;
	int exp_;
	int max_exp_;
	Map *map_;
	Inventory *inventory_;
};

#endif