#include "option_cursor.hpp"

OptionCursor::OptionCursor():
	pos_(0),
	options_str_(),
	options_id_(),
	length_(0)
{
}

OptionCursor::~OptionCursor(){
}

int OptionCursor::get_pos(){
	return pos_;
}

int OptionCursor::get_length(){
	return length_;
}

const char *OptionCursor::get_option_str(int pos){
	return options_str_[pos].c_str();
}

int OptionCursor::get_option_id(){
	return options_id_[pos_];
}

void OptionCursor::add_option(const char *option_str, int option_id){
	options_str_.push_back(option_str);
	options_id_.push_back(option_id);
	length_ = options_str_.size();
}

void OptionCursor::reset(){
	pos_ = 0;
	options_str_.clear();
	options_id_.clear();
	length_ = 0;
}

void OptionCursor::move_up(){
	if(pos_ > 0){
		pos_--;
	}
}

void OptionCursor::move_down(){
	if(pos_ < length_ - 1){
		pos_++;
	}
}

void OptionCursor::set_position(int pos){
	if(pos >= 0 && pos < length_ - 1){
		pos_ = pos;
	}
}
