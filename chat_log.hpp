#ifndef CHAT_LOG_HPP
#define CHAT_LOG_HPP

class ChatLog{
public:
	ChatLog(int line_width, int buffer_size);
	~ChatLog();
	
	void add_line(const char *line);
	
	/*returns the line in descending chronological order*/
	const char *get_line(int line_number);
	int get_num_used_lines();
	void clear();
private:
	int line_width_;
	int buffer_size_;
	int next_line_;
	int num_used_lines_;
	char **lines_;
};

#endif