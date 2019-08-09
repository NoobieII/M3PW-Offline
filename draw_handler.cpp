#include "camera_offset.hpp"
#include "constants.hpp"
#include "error_report.hpp"
#include "inventory.hpp"
#include "item.hpp"
#include "item_data.hpp"
#include "item_data_provider.hpp"
#include "mapdata.hpp"
#include "player.hpp"
#include "pw_graphics.h"
#include "uiinfo.hpp"

#include <string.h>

void UIInfo::draw_start_quit_screen(){
	PWG_draw_str(graphics_ctx_, "M3PW Project", 33, 0, 80, 1, 80);
	PWG_draw_str(graphics_ctx_, "Start Game", 1, 10, 80, 1, 80);
	PWG_draw_str(graphics_ctx_, "Quit Game", 1, 11, 80, 1, 80);
	PWG_draw_str(graphics_ctx_, ">", 0, 10 + start_screen_cursor_.get_pos(), 1, 1, 1);
}

void UIInfo::draw_in_game(){
	int i;
	PWGraphicsContext *map_screen, *chat_box;
	char stat_hp[12], stat_mp[12], stat_exp[22], stat_line[SCREEN_WIDTH + 1];
	float exp_percentage;
	
	map_screen = PWG_get_context(FRAMERATE, SCREEN_WIDTH, SCREEN_HEIGHT - 5, ' ');
	chat_box = PWG_get_context(FRAMERATE, SCREEN_WIDTH, 3, ' ');
	
	/*Draw the map elements*/
	for(i = 0; i < map_data_->blocks.size(); ++i){
		PWG_draw_bytes(
			map_screen,
			" + +++ + ",
			map_data_->blocks[i].x * 3 - camera_offset_x(),
			map_data_->blocks[i].y * 3 - camera_offset_y(),
			3,
			3,
			0);
	}
	
	/*Draw the portals*/
	for(i = 0; i < map_data_->portals.size(); ++i){
		PWG_draw_bytes(
			map_screen,
			" | \\|/-+-",
			map_data_->portals[i].x * 3 - camera_offset_x(),
			map_data_->portals[i].y * 3 - camera_offset_y(),
			3,
			3,
			0);
	}
	
	/*draw the player*/
	PWG_draw_bytes(
		map_screen,
		"io -H- M ",
		player_->get_x_position() * 3 - camera_offset_x(),
		player_->get_y_position() * 3 - camera_offset_y(),
		3,
		3,
		0);
	
	/*draw to the main graphics context*/
	PWG_draw_context(
		graphics_ctx_,
		map_screen,
		0,
		0,
		0,
		0,
		map_screen->width,
		map_screen->height);
	
	/*draw the chat log/box*/
	for(i = 0; i < 3; ++i){
		PWG_draw_str(
			graphics_ctx_,
			chat_log_.get_line(2 - i),
			0,
			SCREEN_HEIGHT - 5 + i,
			SCREEN_WIDTH,
			1,
			SCREEN_WIDTH);
	}
	PWG_draw_str(
		graphics_ctx_,
		"[All]",
		0,
		SCREEN_HEIGHT - 2,
		5,
		1,
		5);
	PWG_draw_str(
		graphics_ctx_,
		chat_box_.get_str(),
		6,
		SCREEN_HEIGHT - 2,
		SCREEN_WIDTH,
		1,
		SCREEN_WIDTH);
	
	/*draw the stuff on the bottom line (player level, hp, mp, exp)*/
	sprintf(stat_hp, "%d/%d", player_->get_hp(), player_->get_max_hp());
	sprintf(stat_mp, "%d/%d", player_->get_mp(), player_->get_max_mp());
	sprintf(stat_exp, "%d/%d", player_->get_exp(), player_->get_max_exp());
	exp_percentage = 100.0 * player_->get_exp() / player_->get_max_exp();
	sprintf(stat_line,
		"Lv %-3d HP: %-11s MP: %-11s EXP: %-21s (%2.2f%%)",
		1,
		stat_hp,
		stat_mp,
		stat_exp,
		exp_percentage);
	
	PWG_draw_str(graphics_ctx_, stat_line, 0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, 1, SCREEN_WIDTH);
	
	PWG_free_context(map_screen);
	PWG_free_context(chat_box);
}

void UIInfo::draw_map_transition(){
	const char *last_screen;
	/*draw the previous screen*/
	
	last_screen = PWG_last_render(graphics_ctx_);
	PWG_draw_bytes(graphics_ctx_, last_screen, 0, 0, 80, 50, 0);
}

void UIInfo::draw_text_in_game(){
	draw_in_game();
	
	/*show the blinking cursor*/
	PWG_draw_bytes(
		graphics_ctx_,
		"_\0",
		6 + chat_box_.get_pos(),
		SCREEN_HEIGHT - 2,
		1,
		1,
		PWG_current_frame(graphics_ctx_) % 2);
}

void UIInfo::draw_inventory(){
	int i;
	Inventory *inv;
	std::shared_ptr<Item> item;
	ItemData *item_data;
	
	inv = player_->get_inventory();
	
	PWG_draw_str(
		graphics_ctx_,
		"Inventory",
		1,
		1,
		SCREEN_WIDTH,
		1,
		SCREEN_WIDTH
	);
	
	/*draw the list of items and item amounts*/
	for(i = 0; inv->get_item_by_slot(i) && i < 10; ++i){
		item = inventory_->get_item_by_slot(i);
		item_data = ItemDataProvider::get_instance()->get_item_data_by_id(item->get_id());
		
		PWG_draw_str(
			graphics_ctx_,
			item_data->name.c_str(),
			1,
			3 + i,
			SCREEN_WIDTH,
			1,
			SCREEN_WIDTH
		);
		
		if(item->get_amount() > 1){
			PWG_draw_str(
				graphics_ctx_,
				"x",
				34,
				3 + i,
				1,
				1,
				1
			);
			
			PWG_draw_int_left_end(
				graphics_ctx_,
				item->get_amount(),
				35,
				3 + i
			);
		}
	}
	
	/*draw the item description for the currently selected item (if there is any)*/
	item = inv->get_item_by_slot(inventory_item_cursor_.get_pos());
	if(item){
		item_data = ItemDataProvider::get_instance()->get_item_data_by_id(item->get_id());
		
		PWG_draw_str_wrapped(
			graphics_ctx_,
			item_data->desc.c_str(),
			39,
			3,
			40,
			10,
			256
		);
	}
	
	/*draw the cursor*/
	PWG_draw_str(
		graphics_ctx_,
		">",
		0,
		3 + inventory_item_cursor_.get_pos(),
		1,
		1,
		1);
}

void UIInfo::draw_inventory_option(){
	int i;
	
	draw_inventory();
	
	/*draw empty square under the options shown*/
	PWG_draw_sqr(graphics_ctx_, ' ', 5, 3 + inventory_item_cursor_.get_pos(), 40, inventory_option_cursor_.get_length());
	
	for(i = 0; i < inventory_option_cursor_.get_length(); ++i){
		/*draw the option strings*/
		PWG_draw_str(
			graphics_ctx_,
			inventory_option_cursor_.get_option_str(i),
			6,
			3 + inventory_item_cursor_.get_pos() + i,
			39,
			1,
			39);
	}
	
	/*draw the cursor*/
	PWG_draw_str(
		graphics_ctx_,
		">",
		5,
		3 + inventory_item_cursor_.get_pos() + inventory_option_cursor_.get_pos(),
		1,
		1,
		1);
}
