#include "constants.hpp"
#include "inventory.hpp"
#include "player.hpp"

Player::Player():
	id_(1),
	stance_(PLAYER_STANCE_UP),
	x_(0),
	y_(0),
	hp_(30),
	max_hp_(30),
	mp_(5),
	max_mp_(5),
	exp_(0),
	max_exp_(50),
	map_(nullptr),
	inventory_(new Inventory(20))
{
}

Player::~Player(){
}

int Player::get_id(){
	return id_;
}

void Player::set_stance(int stance){
	stance_ = stance;
}

int Player::get_stance(){
	return stance_;
}

void Player::set_map(Map *m){
	map_ = m;
}

Map *Player::get_map(){
	return map_;
}

void Player::set_x_position(int x){
	x_ = x;
}

int Player::get_x_position(){
	return x_;
}

void Player::set_y_position(int y){
	y_ = y;
}

int Player::get_y_position(){
	return y_;
}

void Player::set_hp(int hp){
	hp_ = hp;
}

int Player::get_hp(){
	return hp_;
}

void Player::set_max_hp(int max_hp){
	max_hp_ = max_hp;
}

int Player::get_max_hp(){
	return max_hp_;
}

void Player::set_mp(int mp){
	mp_ = mp;
}

int Player::get_mp(){
	return mp_;
}

void Player::set_max_mp(int max_mp){
	max_mp_ = max_mp;
}

int Player::get_max_mp(){
	return max_mp_;
}

void Player::set_exp(int exp){
	exp_ = exp;
}

int Player::get_exp(){
	return exp_;
}

void Player::set_max_exp(int max_exp){
	max_exp_ = max_exp;
}

int Player::get_max_exp(){
	return max_exp_;
}

Inventory *Player::get_inventory(){
	return inventory_;
}
