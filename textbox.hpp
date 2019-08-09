#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

//a class used for anything that has text input
class TextBox{
public:
	TextBox(int size);
	~TextBox();
	
	void backspace();
	void del();
	void add_char(char ch);
	void move_right();
	void move_left();
	void replace(const char *str);
	void clear();
	
	int get_pos();
	const char* get_str();
private:
	int size_;
	int pos_;
	int len_;
	char* str_;
};

#endif