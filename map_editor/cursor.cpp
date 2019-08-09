#include "cursor.hpp"

Cursor::Cursor(int max_val, bool wrap):
	length_(max_val),
	pos_(0),
	is_wrap_(wrap)
{
}

Cursor::~Cursor(){
}

void Cursor::move_up(){
	if(is_wrap_){
		pos_ = (pos_ - 1 + length_) % length_;
	}
	else if(pos_ > 0){
		pos_ = pos_ - 1;
	}
}

void Cursor::move_down(){
	if(is_wrap_){
		pos_ = (pos_ + 1) % length_;
	}
	else if(pos_ < length_ - 1){
		pos_ = pos_ + 1;
	}
}

void Cursor::resize(int max_val){
	if(max_val < 1){
		return;
	}
	
	length_ = max_val;
	if(pos_ >= max_val){
		pos_ = max_val - 1;
	}
}

int Cursor::get_pos(){
	return pos_;
}

void Cursor::reset(){
	pos_ = 0;
}