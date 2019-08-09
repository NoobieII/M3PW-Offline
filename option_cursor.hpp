#ifndef OPTION_CURSOR_HPP
#define OPTION_CURSOR_HPP

#include <vector>
#include <string>

class OptionCursor{
public:
	OptionCursor();
	~OptionCursor();
	
	//get position
	int get_pos();
	int get_length();
	const char *get_option_str(int pos);
	int get_option_id();
	
	void add_option(const char *option_str, int option_id);
	
	//set position to 0, remove all option strings
	void reset();
	
	void move_up();
	void move_down();
	void set_position(int pos);
private:
	int pos_;
	std::vector<std::string> options_str_;
	std::vector<int> options_id_;
	int length_;
};

#endif