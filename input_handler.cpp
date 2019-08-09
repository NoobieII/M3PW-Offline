#include "camera_offset.hpp"
#include "constants.hpp"
#include "inventory.hpp"
#include "item_data.hpp"
#include "item_data_provider.hpp"
#include "key_event.hpp"
#include "map.hpp"
#include "mapdata.hpp"
#include "player.hpp"
#include "pw_graphics.h"
#include "uiinfo.hpp"

#include <string.h>

void UIInfo::handle_input_start_quit_screen(char input){
	switch(input){
	case KeyEvent::KU:
		start_screen_cursor_.move_up();
		break;
	case KeyEvent::KD:
		start_screen_cursor_.move_down();
		break;
	case KeyEvent::ENT:
		/*execute the option*/
		switch(start_screen_cursor_.get_pos()){
		case 0:
			/*initialize game*/
			map_ = new Map(0);
			
			map_data_ = new MapData;
			map_data_->read_from_bytes("data/maps/10000000", 2048);
			
			player_ = new Player;
			map_->add_player(player_);
			
			player_->set_x_position(13);
			player_->set_y_position(8);
			
			/*correct camera offset*/
			camera_offset_set(player_, map_data_);
			
			/*initialize inventory interface*/
			inventory_ = player_->get_inventory();
			
			state_ = STATE_IN_GAME;
			break;
		case 1:
			is_open_ = 0;
			break;
		}
		break;
	case KeyEvent::ESC:
		is_open_ = 0;
		break;
	}
}

void UIInfo::handle_input_in_game(char input){
	int i;
	int map_dest_id;
	int map_dest_portal_id;
	int is_at_portal = 0;
	char mapdata_filename[128];
	
	//for inventory screen
	std::shared_ptr<Item> item;
	ItemData *item_data;
	
	switch(input){
	case KeyEvent::KU:
		/*player stance in up position*/
		if(player_->get_stance() == PLAYER_STANCE_UP 
			|| player_->get_stance() == PLAYER_STANCE_MOVE_UP){
			player_->set_stance(PLAYER_STANCE_MOVE_UP);
		}
		else{
			player_->set_stance(PLAYER_STANCE_UP);
		}
		break;
	case KeyEvent::KD:
		/*player stance in down position*/
		if(player_->get_stance() == PLAYER_STANCE_DOWN
			|| player_->get_stance() == PLAYER_STANCE_MOVE_DOWN){
			player_->set_stance(PLAYER_STANCE_MOVE_DOWN);
		}
		else{
			player_->set_stance(PLAYER_STANCE_DOWN);
		}
		break;
	case KeyEvent::KL:
		/*player stance in left position*/
		if(player_->get_stance() == PLAYER_STANCE_LEFT
			|| player_->get_stance() == PLAYER_STANCE_MOVE_LEFT){
			player_->set_stance(PLAYER_STANCE_MOVE_LEFT);
		}
		else{
			player_->set_stance(PLAYER_STANCE_LEFT);
		}
		break;
	case KeyEvent::KR:
		/*player stance in right position*/
		if(player_->get_stance() == PLAYER_STANCE_RIGHT
			|| player_->get_stance() == PLAYER_STANCE_MOVE_RIGHT){
			player_->set_stance(PLAYER_STANCE_MOVE_RIGHT);
		}
		else{
			player_->set_stance(PLAYER_STANCE_RIGHT);
		}
		break;
	case ' ':
		/*player goes in portal*/
		/*check all portals on the map, if their position equals player position*/
		for(i = 0; i < map_data_->portals.size(); ++i){
			if(map_data_->portals[i].x == player_->get_x_position() && map_data_->portals[i].y == player_->get_y_position()){
				is_at_portal = 1;
				map_dest_portal_id = map_data_->portals[i].dest_portal_id;
				map_dest_id = map_data_->portals[i].dest_map_id;
				break;
			}
		}
		
		if(!is_at_portal){
			break;
		}
		else{
			/*delete current map*/
			map_->remove_player(player_->get_id());
			delete map_;
			
			/*load destination map data*/
			sprintf(mapdata_filename, "data/maps/%d", map_dest_id);
			map_data_->read_from_bytes(mapdata_filename, 2048);
			
			map_ = new Map(map_dest_id);
			map_->add_player(player_);
			
			/*put player in correct position*/
			for(i = 0; i < map_data_->portals.size(); ++i){
				if(map_data_->portals[i].id == map_dest_portal_id){
					player_->set_x_position(map_data_->portals[i].x);
					player_->set_y_position(map_data_->portals[i].y);
				}
			}
			
			/*correct camera offset*/
			camera_offset_set(player_, map_data_);
			
			map_transition_start_time_ = PWG_current_frame(graphics_ctx_);
			state_ = STATE_MAP_TRANSITION;
		}
		break;
	case KeyEvent::ESC:
		/*quit the game, clean memory*/
		delete player_;
		delete map_;
		delete map_data_;
		
		state_ = STATE_START_QUIT_SCREEN;
		break;
	case KeyEvent::ENT:
		state_ = STATE_TEXT_IN_GAME;
		break;
	case 'i':
	case 'I':
		//prepare the interface 
		
		//get the name of all the items in the inventory
		inventory_item_cursor_.reset();
		inventory_item_cursor_.resize(inventory_->get_num_items());
		
		state_ = STATE_INVENTORY;
		break;
	}
}

void UIInfo::handle_input_map_transition(char input){
	/*input is ignored*/
}

void UIInfo::handle_input_text_in_game(char input){
	const char *chat_box_str;
	
	switch(input){
	case KeyEvent::ESC:
		chat_box_.clear();
		state_ = STATE_IN_GAME;
		break;
	case KeyEvent::ENT:
		chat_box_str = chat_box_.get_str();
		
		/*process command or transmit the message*/
		if(strlen(chat_box_str) > 0){
			if(chat_box_str[0] == '/'){
				/*process command*/
				process_chat_command(chat_box_str + 1);
			}
			else{
				chat_log_.add_line(chat_box_.get_str());
			}
			chat_box_.clear();
		}
		break;
	case KeyEvent::KL:
		chat_box_.move_left();
		break;
	case KeyEvent::KR:
		chat_box_.move_right();
		break;
	case KeyEvent::BKS:
		chat_box_.backspace();
		break;
	}
	if(input > 31){
		chat_box_.add_char(input);
	}
}

void UIInfo::handle_input_inventory(char input){
	std::shared_ptr<Item> item;
	
	switch(input){
	case KeyEvent::KU:
		inventory_item_cursor_.move_up();
		break;
	case KeyEvent::KD:
		inventory_item_cursor_.move_down();
		break;
	case KeyEvent::ENT:
		item = inventory_->get_item_by_slot(inventory_item_cursor_.get_pos());
		
		if(!item){
			break;
		}
		
		populate_item_options(item->get_id());
		
		state_ = STATE_INVENTORY_OPTION;
		break;
	case KeyEvent::ESC:
		state_ = STATE_IN_GAME;
		break;
	}
}

void UIInfo::handle_input_inventory_option(char input){
	switch(input){
	case KeyEvent::KU:
		inventory_option_cursor_.move_up();
		break;
	case KeyEvent::KD:
		inventory_option_cursor_.move_down();
		break;
	case KeyEvent::ESC:
		state_ = STATE_INVENTORY;
		break;
	}
}
