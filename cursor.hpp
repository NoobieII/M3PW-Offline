#ifndef CURSOR_HPP
#define CURSOR_HPP

/*cleaner way to implement cursors instead of using plain integers
 *
 *(December 12, 2018)
 *
 *March 16, 2019 note: max_val in constructor refers to the number of
 *possible cursor positions
 */
 
class Cursor{
public:
	Cursor(int max_val, bool wrap = false);
	~Cursor();
	
	void move_up();
	void move_down();
	int get_pos();
	void resize(int max_val);
	
	//set position to 0
	void reset();
private:
	int pos_;
	int length_;
	bool is_wrap_;
};

#endif
