#include "uiinfo.hpp"
#include "pw_graphics.h"
#include "key_event.hpp"
#include <stdio.h>

UIInfo::UIInfo():
	state_(UI_STATE_EDITOR),
	is_open_(0),
	map_properties_cursor_(4, false),
	id_textbox_(16),
	width_textbox_(8),
	height_textbox_(8),
	element_type_(0),
	cursor_x_(1),
	cursor_y_(1),
	block_id_(1),
	portal_id_(1),
	offset_x_(0),
	offset_y_(0),
	selected_text_(2, true),
	block_edit_index_(0),
	portal_dest_map_id_textbox_(16),
	portal_dest_portal_id_textbox_(16),
	portal_edit_index_(0),
	menu_cursor_(3),
	load_filename_(128),
	save_filename_(128),
	map_data_(),
	graphics_ctx_(PWG_get_context(3, 80, 30, ' '))
{
}

UIInfo::~UIInfo(){
	PWG_free_context(graphics_ctx_);
}

int UIInfo::initialize(const char *filename){
	int highest_id = 0;
	int i;
	
	if(!filename){
		state_ = UI_STATE_MAP_PROPERTIES;
		is_open_ = 1;
		return 0;
	}
	
	if(map_data_.read_from_bytes(filename, 1024*1024) == -1){
		is_open_ = 0;
		return -1;
	}
	
	/*if the file loads successfully, put the filename in savefile textbox*/
	state_ = UI_STATE_EDITOR;
	cursor_x_.resize(map_data_.width);
	cursor_y_.resize(map_data_.height);
	save_filename_.replace(filename);
	
	/*find the highest ids*/
	for(i = 0; i < map_data_.blocks.size(); ++i){
		if(map_data_.blocks[i].id > highest_id){
			highest_id = map_data_.blocks[i].id;
		}
	}
	block_id_ = highest_id + 1;
	
	highest_id = 0;
	for(i = 0; i < map_data_.portals.size(); ++i){
		if(map_data_.portals[i].id > highest_id){
			highest_id = map_data_.portals[i].id;
		}
	}
	portal_id_ = highest_id + 1;
	is_open_ = 1;
	
	return 0;
}

void UIInfo::handle_input(){
	
	if(!kbhit()){
		return;
	}
	
	switch(state_){
	case UI_STATE_MAP_PROPERTIES:
		handle_input_map_properties();
		break;
	case UI_STATE_EDITOR:
		handle_input_editor();
		break;
	case UI_STATE_ELEMENT_PORTAL:
		handle_input_element_portal();
		break;
	case UI_STATE_MENU:
		handle_input_menu();
		break;
	case UI_STATE_TYPE_SAVE_FILENAME:
		handle_input_type_save_filename();
		break;
	case UI_STATE_TYPE_LOAD_FILENAME:
		handle_input_type_load_filename();
		break;
	}
}

void UIInfo::handle_input_map_properties(){
	int buf1, buf2, buf3;
	int success = 1;
	char input = get_char_input();
	
	switch(input){
	case KeyEvent::KU:
		map_properties_cursor_.move_up();
		break;
	case KeyEvent::TAB:
	case KeyEvent::KD:
		map_properties_cursor_.move_down();
		break;
	case KeyEvent::BKS:
		switch(map_properties_cursor_.get_pos()){
		case 0: /*map id*/
			id_textbox_.backspace();
			break;
		case 1: /*map width*/
			width_textbox_.backspace();
			break;
		case 2:	/*map height*/
			height_textbox_.backspace();
			break;
		}
		break;
	case KeyEvent::ENT:
		/*check if all three fields are valid integers*/
		
		if(sscanf(id_textbox_.get_str(), "%d", &buf1) != 1){
			success = 0;
		}
		if(sscanf(width_textbox_.get_str(), "%d", &buf2) != 1){
			success = 0;
		}
		if(sscanf(height_textbox_.get_str(), "%d", &buf3) != 1){
			success = 0;
		}
		
		if(success){
			map_data_.id = buf1;
			map_data_.width = buf2;
			map_data_.height = buf3;
			
			cursor_x_.resize(map_data_.width);
			cursor_y_.resize(map_data_.height);
			state_ = UI_STATE_EDITOR;
		}
		
		break;
	}
	
	/*for readable characters, add to textbox*/
	if(input > 31){
		switch(map_properties_cursor_.get_pos()){
		case 0:
			id_textbox_.add_char(input);
			break;
		case 1:
			width_textbox_.add_char(input);
			break;
		case 2:
			height_textbox_.add_char(input);
			break;
		}
	}
}

void UIInfo::handle_input_editor(){
	int i;
	char block_on_cursor = 0;
	char portal_on_cursor = 0;
	char input;
	MapBlock block;
	MapPortal portal;
	
	input = get_char_input();
	
	/*TODO: implement screen offset if the map is bigger than the screen*/
	switch(input){
	case KeyEvent::KU:
		cursor_y_.move_up();
		if(offset_y_ > cursor_y_.get_pos()){
			offset_y_--;
		}
		break;
	case KeyEvent::KD:
		cursor_y_.move_down();
		if(cursor_y_.get_pos() > offset_y_ + 8){
			offset_y_++;
		}
		break;
	case KeyEvent::KL:
		cursor_x_.move_up();
		if(offset_x_ > cursor_x_.get_pos()){
			offset_x_--;
		}
		break;
	case KeyEvent::KR:
		cursor_x_.move_down();
		if(cursor_x_.get_pos() > offset_x_ + 25){
			offset_x_++;
		}
		break;
	case KeyEvent::TAB:	/*switch map element*/
		switch(element_type_){
		case 0:
			element_type_ = 1;
			break;
		case 1:
			element_type_ = 0;
			break;
		default:
			element_type_  = 0;
			break;
		}
		break;
	case KeyEvent::ENT: /*place an element*/
		/*check if there is an element already placed where the cursor is*/
		for(i = 0; i < map_data_.blocks.size(); ++i){
			if(map_data_.blocks[i].x == cursor_x_.get_pos() && map_data_.blocks[i].y == cursor_y_.get_pos()){
				block_on_cursor = 1;
			}
		}
		for(i = 0; i < map_data_.portals.size(); ++i){
			if(map_data_.portals[i].x == cursor_x_.get_pos() && map_data_.portals[i].y == cursor_y_.get_pos()){
				portal_on_cursor = 1;
			}
		}
		
		switch(element_type_){
		case 0:	/*block element*/
			if(block_on_cursor){
				break;
			}
			block = {block_id_++, cursor_x_.get_pos(), cursor_y_.get_pos()};
			map_data_.blocks.push_back(block);
			
			break;
		case 1: /*portal element*/
			if(portal_on_cursor){
				break;
			}
			portal = {portal_id_++, cursor_x_.get_pos(), cursor_y_.get_pos(), 0, 0};
			map_data_.portals.push_back(portal);
			
			break;
		}
		break;
	case KeyEvent::DEL: /*remove an element*/
		/*check if there is an element already placed where the cursor is*/
		for(i = 0; i < map_data_.blocks.size(); ++i){
			if(map_data_.blocks[i].x == cursor_x_.get_pos() && map_data_.blocks[i].y == cursor_y_.get_pos()){
				while(i < map_data_.blocks.size() - 1){
					map_data_.blocks[i] = map_data_.blocks[i + 1];
					i++;
				}
				map_data_.blocks.pop_back();
			}
		}
		for(i = 0; i < map_data_.portals.size(); ++i){
			if(map_data_.portals[i].x == cursor_x_.get_pos() && map_data_.portals[i].y == cursor_y_.get_pos()){
				while(i < map_data_.portals.size() - 1){
					map_data_.portals[i] = map_data_.portals[i + 1];
					i++;
				}
				map_data_.portals.pop_back();
			}
		}
		
		break;
	case ' ': /*edit an element's attributes*/
		/*check if there is an element already placed where the cursor is*/
		for(i = 0; i < map_data_.blocks.size(); ++i){
			if(map_data_.blocks[i].x == cursor_x_.get_pos() && map_data_.blocks[i].y == cursor_y_.get_pos()){
				block_on_cursor = 1;
				block_edit_index_ = i;
			}
		}
		for(i = 0; i < map_data_.portals.size(); ++i){
			if(map_data_.portals[i].x == cursor_x_.get_pos() && map_data_.portals[i].y == cursor_y_.get_pos()){
				portal_on_cursor = 1;
				portal_edit_index_ = i;
			}
		}
		
		switch(element_type_){
		case 0: /*edit block element (no attributes to change yet)*/
			break;
		case 1: /*edit portal element*/
			if(portal_on_cursor){
				selected_text_.resize(2);
				state_ = UI_STATE_ELEMENT_PORTAL;
			}
			break;
		}
		break;
	case KeyEvent::ESC:
		state_ = UI_STATE_MENU;
		break;
	}
}

void UIInfo::handle_input_element_portal(){
	char input;
	char success = 1;
	int buf1, buf2;
	
	input = get_char_input();
	
	switch(input){
	case KeyEvent::BKS:
		switch(selected_text_.get_pos()){
		case 0: /*edit dest map id*/
			portal_dest_map_id_textbox_.backspace();
			break;
		case 1: /*edit dest portal id*/
			portal_dest_portal_id_textbox_.backspace();
			break;
		}
		break;
	case KeyEvent::TAB: /*switch text boxes*/
		selected_text_.move_down();
		break;
	case KeyEvent::ENT:
		if(sscanf(portal_dest_map_id_textbox_.get_str(), "%d", &buf1) != 1){
			success = 0;
		}
		if(sscanf(portal_dest_portal_id_textbox_.get_str(), "%d", &buf2) != 1){
			success = 0;
		}
		
		if(!success){
			break;
		}
		
		map_data_.portals[portal_edit_index_].dest_map_id = buf1;
		map_data_.portals[portal_edit_index_].dest_portal_id = buf2;
		portal_dest_map_id_textbox_.clear();
		portal_dest_portal_id_textbox_.clear();
		state_ = UI_STATE_EDITOR;
		break;
	case KeyEvent::ESC:
		portal_dest_map_id_textbox_.clear();
		portal_dest_portal_id_textbox_.clear();
		state_ = UI_STATE_EDITOR;
		break;
	}
	
	if(input > 31){
		switch(selected_text_.get_pos()){
		case 0: /*edit dest map id*/
			portal_dest_map_id_textbox_.add_char(input);
			break;
		case 1: /*edit dest portal id*/
			portal_dest_portal_id_textbox_.add_char(input);
			break;
		}
	}
}

void UIInfo::handle_input_menu(){
	char input;
	
	input = get_char_input();
	
	switch(input){
	case KeyEvent::KU:
		menu_cursor_.move_up();
		break;
	case KeyEvent::KD:
		menu_cursor_.move_down();
		break;
	case KeyEvent::ESC:
		state_ = UI_STATE_EDITOR;
		break;
	case KeyEvent::ENT:
		switch(menu_cursor_.get_pos()){
		case 0: /*save file*/
			state_ = UI_STATE_TYPE_SAVE_FILENAME;
			break;
		case 1: /*load file*/
			state_ = UI_STATE_TYPE_LOAD_FILENAME;
			break;
		case 2: /*quit editor*/
			is_open_ = 0;
			break;
		}
		break;
	}
}

void UIInfo::handle_input_type_save_filename(){
	char input;
	
	input = get_char_input();
	
	switch(input){
	case KeyEvent::BKS:
		save_filename_.backspace();
		break;
	case KeyEvent::ENT:
		if(map_data_.write_to_bytes(save_filename_.get_str(), 1024*1024) == 0){
			state_ = UI_STATE_EDITOR;
		}
		break;
	case KeyEvent::ESC:
		state_ = UI_STATE_EDITOR;
		break;
	}
	
	if(input > 31){
		save_filename_.add_char(input);
	}
}

void UIInfo::handle_input_type_load_filename(){
	char input;
	int highest_id = 0;
	int i;
	
	input = get_char_input();
	
	switch(input){
	case KeyEvent::BKS:
		load_filename_.backspace();
		break;
	case KeyEvent::ENT:
		if(map_data_.read_from_bytes(load_filename_.get_str(), 999999) == 0){
			/*change map editor bounds*/
			cursor_x_.resize(map_data_.width);
			cursor_y_.resize(map_data_.height);
			save_filename_.replace(load_filename_.get_str());
			
			/*find the highest ids*/
			for(i = 0; i < map_data_.blocks.size(); ++i){
				if(map_data_.blocks[i].id > highest_id){
					highest_id = map_data_.blocks[i].id;
				}
			}
			block_id_ = highest_id + 1;
			
			highest_id = 0;
			for(i = 0; i < map_data_.portals.size(); ++i){
				if(map_data_.portals[i].id > highest_id){
					highest_id = map_data_.portals[i].id;
				}
			}
			portal_id_ = highest_id + 1;
			
			state_ = UI_STATE_EDITOR;
		}
		break;
	case KeyEvent::ESC:
		state_ = UI_STATE_EDITOR;
		break;
	}
	
	if(input > 31){
		load_filename_.add_char(input);
	}
}

void UIInfo::draw(){
	switch(state_){
	case UI_STATE_MAP_PROPERTIES:
		draw_map_properties();
		break;
	case UI_STATE_EDITOR:
		draw_editor();
		break;
	case UI_STATE_ELEMENT_PORTAL:
		draw_element_portal();
		break;
	case UI_STATE_MENU:
		draw_menu();
		break;
	case UI_STATE_TYPE_SAVE_FILENAME:
		draw_type_save_filename();
		break;
	case UI_STATE_TYPE_LOAD_FILENAME:
		draw_type_load_filename();
		break;
	}
	
	PWG_render(graphics_ctx_);
}

void UIInfo::draw_map_properties(){
	PWG_draw_str(graphics_ctx_, ">", 0, map_properties_cursor_.get_pos(), 1, 1, 1);
	PWG_draw_str(graphics_ctx_, "ID:     ___", 1, 0, 100, 1, 100);
	PWG_draw_str(graphics_ctx_, "WIDTH:  ___", 1, 1, 100, 1, 100);
	PWG_draw_str(graphics_ctx_, "HEIGHT: ___", 1, 2, 100, 1, 100);
	PWG_draw_str(graphics_ctx_, id_textbox_.get_str(), 9, 0, 16, 1, 16);
	PWG_draw_str(graphics_ctx_, width_textbox_.get_str(), 9, 1, 16, 1, 16);
	PWG_draw_str(graphics_ctx_, height_textbox_.get_str(), 9, 2, 16, 1, 16);
}

void UIInfo::draw_editor(){
	char str[80];
	PWSprite *cursor_sprite = NULL;
	int i;
	
	sprintf(str, "Map id=%d width=%d height =%d", map_data_.id, map_data_.width, map_data_.height);
	PWG_draw_str(graphics_ctx_, str, 0, 0, 80, 1, 80);
	
	
	switch(element_type_){
	case 0: /*block element*/
		for(i = 0; i < map_data_.blocks.size(); ++i){
			PWG_draw_bytes(
				graphics_ctx_,
				" + +++ + ",
				map_data_.blocks[i].x * 3 - offset_x_ * 3,
				map_data_.blocks[i].y * 3 - offset_y_ * 3 + 1,
				3,
				3,
				0);
		}
		/*show block element properties if cursor is on it.*/
		for(i = 0; i < map_data_.blocks.size(); ++i){
			if(map_data_.blocks[i].x == cursor_x_.get_pos() && map_data_.blocks[i].y == cursor_y_.get_pos()){
				sprintf(str, "Block id=%d", map_data_.blocks[i].id);
				PWG_draw_str(graphics_ctx_, str, 0, 29, 80, 1, 80);
			}
		}
		break;
	case 1: /*portal element*/
		for(i = 0; i < map_data_.portals.size(); ++i){
			PWG_draw_bytes(
				graphics_ctx_, 
				" | \\|/- -",
				map_data_.portals[i].x * 3 - offset_x_ * 3,
				map_data_.portals[i].y * 3 - offset_y_ * 3 + 1,
				3,
				3,
				0);
		}
		/*show portal element properties if cursor is on it*/
		for(i = 0; i < map_data_.portals.size(); ++i){
			if(map_data_.portals[i].x == cursor_x_.get_pos() && map_data_.portals[i].y == cursor_y_.get_pos()){
				sprintf(str, "Portal id=%d dest_map_id=%d dest_portal_id=%d", map_data_.portals[i].id, map_data_.portals[i].dest_map_id, map_data_.portals[i].dest_portal_id);
				PWG_draw_str(graphics_ctx_, str, 0, 29, 80, 1, 80);
			}
		}
		break;
	}
	
	/*draw the cursor*/
	PWG_draw_bytes(
		graphics_ctx_, " . . . . ",
		cursor_x_.get_pos() * 3 - offset_x_ * 3,
		cursor_y_.get_pos() * 3 - offset_y_ * 3 + 1,
		3,
		3,
		0);
}

void UIInfo::draw_element_portal(){
	PWG_draw_str(graphics_ctx_, ">", 0, selected_text_.get_pos(), 1, 1, 1);
	PWG_draw_str(graphics_ctx_, "dest_map:    ____", 1, 0, 100, 1, 100);
	PWG_draw_str(graphics_ctx_, "dest_portal: ____", 1, 1, 100, 1, 100);
	PWG_draw_str(graphics_ctx_, portal_dest_map_id_textbox_.get_str(), 14, 0, 80, 1, 80);
	PWG_draw_str(graphics_ctx_, portal_dest_portal_id_textbox_.get_str(), 14, 1, 80, 1, 80);
}

void UIInfo::draw_menu(){
	PWG_draw_str(graphics_ctx_, ">", 0, menu_cursor_.get_pos(), 1, 1, 1);
	PWG_draw_str(graphics_ctx_, "Save File", 1, 0, 80, 1, 80);
	PWG_draw_str(graphics_ctx_, "Load File", 1, 1, 80, 1, 80);
	PWG_draw_str(graphics_ctx_, "Quit", 1, 2, 80, 1, 80);
}

void UIInfo::draw_type_save_filename(){
	PWG_draw_str(graphics_ctx_, "Save to: _______", 0, 0, 79, 2, 160);
	PWG_draw_str(graphics_ctx_, save_filename_.get_str(), 9, 0, 80, 1, 80);
}

void UIInfo::draw_type_load_filename(){
	PWG_draw_str(graphics_ctx_, "Load from: _______", 0, 0, 79, 2, 160);
	PWG_draw_str(graphics_ctx_, load_filename_.get_str(), 11, 0, 80, 1, 80);
}

int UIInfo::is_open(){
	return is_open_;
}