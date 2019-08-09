#include "camera_offset.hpp"
#include "constants.hpp"
#include "error_report.hpp"
#include "key_event.hpp"
#include "item.hpp"
#include "item_data.hpp"
#include "item_data_provider.hpp"
#include "inventory.hpp"
#include "map.hpp"
#include "mapdata.hpp"
#include "player.hpp"
#include "pw_graphics.h"
#include "uiinfo.hpp"

#include <cstring>
#include <memory>

UIInfo *UIInfo::singleton_ = 0;

UIInfo *UIInfo::get_instance(){
	if(!singleton_){
		singleton_ = new UIInfo;
		return singleton_;
	}
	return singleton_;
}

void UIInfo::handle_input(){
	char input;
	
	while(kbhit()){
		input = get_char_input();
		
		//handle input
		switch(state_){
		case STATE_START_QUIT_SCREEN:
			handle_input_start_quit_screen(input);
			break;
		case STATE_IN_GAME:
			handle_input_in_game(input);
			break;
		case STATE_MAP_TRANSITION:
			handle_input_map_transition(input);
			break;
		case STATE_TEXT_IN_GAME:
			handle_input_text_in_game(input);
			break;
		case STATE_INVENTORY:
			handle_input_inventory(input);
			break;
		case STATE_INVENTORY_OPTION:
			handle_input_inventory_option(input);
			break;
		}
	}
}

void UIInfo::draw(){
	switch(state_){
	case STATE_START_QUIT_SCREEN:
		draw_start_quit_screen();
		break;
	case STATE_IN_GAME:
		draw_in_game();
		break;
	case STATE_MAP_TRANSITION:
		draw_map_transition();
		break;
	case STATE_TEXT_IN_GAME:
		draw_text_in_game();
		break;
	case STATE_INVENTORY:
		draw_inventory();
		break;
	case STATE_INVENTORY_OPTION:
		draw_inventory_option();
		break;
	}
}

/*returns 0 on success, -1 on failure*/
int UIInfo::init(){
	graphics_ctx_ = PWG_get_context(FRAMERATE, SCREEN_WIDTH, SCREEN_HEIGHT, ' ');
	
	if(!graphics_ctx_){
		return -1;
	}
	
	return 0;
}

int UIInfo::is_open(){
	return is_open_;
}

UIInfo::UIInfo():
	state_(STATE_START_QUIT_SCREEN),
	is_open_(1),
	start_screen_cursor_(2, false),
	player_(0),
	map_(0),
	map_data_(0),
	map_transition_start_time_(0),
	chat_box_(80),
	chat_log_(80, 3),
	inventory_(nullptr),
	inventory_item_cursor_(0),
	inventory_option_cursor_(),
	graphics_ctx_(0)
{
}
	
UIInfo::~UIInfo(){
}

void UIInfo::pass_tick(int t){
	int dest_pos_x, dest_pos_y;
	int i;
	int is_player_move_valid = 1;
	
	while(t > 0){
		switch(state_){
		case STATE_START_QUIT_SCREEN:
			break;
		case STATE_IN_GAME:
			/*move player in motion, change state from moving to standing*/
			switch(player_->get_stance()){
			case PLAYER_STANCE_MOVE_UP:
				dest_pos_x = player_->get_x_position();
				dest_pos_y = player_->get_y_position() - 1;
				player_->set_stance(PLAYER_STANCE_UP);
				break;
			case PLAYER_STANCE_MOVE_DOWN:
				dest_pos_x = player_->get_x_position();
				dest_pos_y = player_->get_y_position() + 1;
				player_->set_stance(PLAYER_STANCE_DOWN);
				break;
			case PLAYER_STANCE_MOVE_LEFT:
				dest_pos_x = player_->get_x_position() - 1;
				dest_pos_y = player_->get_y_position();
				player_->set_stance(PLAYER_STANCE_LEFT);
				break;
			case PLAYER_STANCE_MOVE_RIGHT:
				dest_pos_x = player_->get_x_position() + 1;
				dest_pos_y = player_->get_y_position();
				player_->set_stance(PLAYER_STANCE_RIGHT);
				break;
			default:
				break;
			}
			
			/*check if the player is not going out of boundary*/
			if((dest_pos_y < 0) || (dest_pos_y >= map_data_->height) || (dest_pos_x < 0) || (dest_pos_x >= map_data_->width)){
				break;
			}
			
			/*check if the player is not colliding with any map blocks*/
			for(i = 0; i < map_data_->blocks.size(); ++i){
				if((dest_pos_x == map_data_->blocks[i].x) && (dest_pos_y == map_data_->blocks[i].y)){
					is_player_move_valid = 0;
					break;
				}
			}
			
			if(is_player_move_valid){
				player_->set_x_position(dest_pos_x);
				player_->set_y_position(dest_pos_y);
			}
			
			camera_offset_update();
			break;
		case STATE_MAP_TRANSITION:
			/*wait for a small interval (in the online application, it would wait
			 *a minimum amount of time or the amount of time information takes
			 *to travel, whichever is longer*/
			if(PWG_current_frame(graphics_ctx_) > map_transition_start_time_ + 3){
				state_ = STATE_IN_GAME;
			}
			
			break;
		case STATE_TEXT_IN_GAME:
			break;
		}
		t--;
	}
}

void UIInfo::process_chat_command(const char *str){
	char arg1[80], arg2[80], arg3[80];
	int int1, int2, int3;
	ItemData *item_data;
	std::shared_ptr<Item> give_item;
	
	if(sscanf(str, "%s", arg1) != 1){
		return;
	}
	
	/*give <item_id> <quantity>*/
	if(strcmp(arg1, "give") == 0){
		if(sscanf(str, "%s%d%d", arg1, &int1, &int2) != 3){
			return;
		}
		
		/*find the item with the id*/
		item_data = ItemDataProvider::get_instance()->get_item_data_by_id(int1);
		
		if(item_data == nullptr){
			log_error("UIInfo::process_chat_command - item data with id %d not found", int1);
			return;
		}
		
		give_item = std::make_shared<Item>(int1, 1);
		
		/*set the quantity*/
		if(item_data->is_stackable && int2 > 0){
			give_item->set_amount(int2);
		}
		
		/*put the item in player inventory*/
		if(player_->get_inventory()->add_item(give_item) == -1){
			log_error("UIInfo::process_chat_command - could not give item %d", int1);
		}
	}
}

void UIInfo::populate_item_options(int item_id){
	ItemData *item_data;
	char option_str[40];
	
	inventory_option_cursor_.reset();
	
	item_data = ItemDataProvider::get_instance()->get_item_data_by_id(item_id);
	if(!item_data){
		log_error("UIInfo::populate_item_options - item data with id %d not found", item_id);
		return;
	}
	
	if(item_id >= 21000000 && item_id < 21010000){
		sprintf(option_str, "Eat %s", item_data->name.c_str());
		inventory_option_cursor_.add_option(option_str, ITEM_OPTION_USE);
	}
	else if(item_id >= 21010000 && item_id < 21020000){
		sprintf(option_str, "Drink %s", item_data->name.c_str());
		inventory_option_cursor_.add_option(option_str, ITEM_OPTION_USE);
	}
	
	inventory_option_cursor_.add_option("Cancel", ITEM_OPTION_CANCEL);
}


PWGraphicsContext *UIInfo::get_graphics_ctx(){
	return graphics_ctx_;
}
