/*Chat log code tested and free of bugs.
 *June 10, 2019*/

#include "chat_log.hpp"
#include <string.h>
#include <stdio.h>

ChatLog::ChatLog(int line_width, int buffer_size):
	line_width_(line_width),
	buffer_size_(buffer_size),
	next_line_(0),
	num_used_lines_(0),
	lines_(0)
{
	int i;
	lines_ = new char*[buffer_size_];
	for(i = 0; i < buffer_size_; ++i){
		lines_[i] = new char[line_width_]();
	}
}
	
ChatLog::~ChatLog(){
	int i;
	for(i = 0; i < buffer_size_; ++i){
		delete[] lines_[i];
	}
	delete[] lines_;
}

void ChatLog::add_line(const char *line){
	int i;
	int line_buffer_length = 1 + (strlen(line) - 1) / (line_width_ - 1);
	
	for(i = 0; i < line_buffer_length; ++i){
		strncpy(lines_[(i + next_line_) % buffer_size_], line + (line_width_ - 1) * i, line_width_ - 1);
		
		if(num_used_lines_ < buffer_size_){
			num_used_lines_++;
		}
	}
	
	next_line_ = (next_line_ + line_buffer_length) % buffer_size_;
}

/*returns the line in descending chronological order*/
const char *ChatLog::get_line(int line_number){
	return lines_[(next_line_ - 1 - line_number + buffer_size_) % buffer_size_];
}

int ChatLog::get_num_used_lines(){
	return num_used_lines_;
}

void ChatLog::clear(){
	int i;
	for(i = 0; i < buffer_size_; ++i){
		memset(lines_[i], 0, 1);
	}
	next_line_ = 0;
	num_used_lines_ = 0;
}
