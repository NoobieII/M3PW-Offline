/*tested and confirmed to work, May 26, 2018*/

#include "constants.hpp"
#include "camera_offset.hpp"
#include "mapdata.hpp"
#include "player.hpp"

#include <stdio.h>

static int offset_x = 0;
static int offset_y = 0;

/*check if the map is static for scrolling*/
static int is_hhold = 0;
static int is_vhold = 0;

static Player *player;
static MapData *map_data;

/*call when player is initially put in the map*/
int camera_offset_set(Player *p, MapData *m){
	if((!p) || (!m)){
		return -1;
	}
	player = p;
	map_data = m;
	if(m->width > (SCREEN_WIDTH / 3)){
		is_hhold = 0;
	}
	else{
		is_hhold = 1;
	}
	if(m->height > ((SCREEN_HEIGHT - 5) / 3)){
		is_vhold = 0;
	}
	else{
		is_vhold = 1;
	}
	
	if(is_hhold){
		offset_x = m->width * 3 / 2 - SCREEN_WIDTH / 2;
	}
	else{
		/*left edge of map*/
		if(p->get_x_position() * 3 < SCREEN_WIDTH / 2){
			offset_x = 0;
		}
		/*middle of map*/
		else if((p->get_x_position() * 3 > SCREEN_WIDTH / 2) && (m->width * 3 - p->get_x_position() * 3 < SCREEN_WIDTH / 2)){
			offset_x = p->get_x_position() * 3 - SCREEN_WIDTH / 2;
		}
		/*right edge of map*/
		else{
			offset_x = m->width * 3 - SCREEN_WIDTH / 2;
		}
	}
	
	if(is_vhold){
		offset_y = m->height * 3 / 2 - (SCREEN_HEIGHT  - 5)/ 2;
	}
	else{
		/*top edge of map*/
		if(p->get_y_position() * 3 < SCREEN_HEIGHT / 2){
			offset_y = 0;
		}
		/*middle of map*/
		else if((p->get_y_position() * 3 > SCREEN_HEIGHT / 2) && (m->height * 3 - p->get_y_position() * 3 < SCREEN_HEIGHT / 2)){
			offset_y = p->get_y_position() * 3 - SCREEN_HEIGHT / 2;
		}
		/*bottom edge of map*/
		else{
			offset_y = m->height * 3 - SCREEN_HEIGHT / 2;
		}
	}
	return 0;
}

/*call each frame after camera_offset_set() is called*/
int camera_offset_update(){
	if((!player) || (!map_data)){
		return -1;
	}
	if(is_hhold){
		offset_x = map_data->width * 3 / 2 - SCREEN_WIDTH / 2;
	}
	else{
		/*move offset to left*/
		if(player->get_x_position() * 3 < offset_x + SCREEN_WIDTH / 2 - OFFSET_TOLERANCE * 3){
			/*if on the left edge of map*/
			if(player->get_x_position() * 3  - SCREEN_WIDTH / 2 / 3 * 3 + OFFSET_TOLERANCE * 3 < 0){
				offset_x = 0;
			}
			/*if the map can still scroll*/
			else{
				offset_x = player->get_x_position() * 3 - SCREEN_WIDTH / 2 / 3 * 3 + OFFSET_TOLERANCE * 3;
			}
		}
		/*dont move offset*/
		else if((player->get_x_position() * 3 - SCREEN_WIDTH / 2 - offset_x) * (player->get_x_position() * 3 - SCREEN_WIDTH / 2 - offset_x)
			< 9 * OFFSET_TOLERANCE * OFFSET_TOLERANCE){
			
		}
		/*move offset to right*/
		else{
			if(player->get_x_position() * 3 + SCREEN_WIDTH / 2 - OFFSET_TOLERANCE * 3 > map_data->width * 3){
				offset_x = map_data->width * 3 - SCREEN_WIDTH / 3 * 3;
			}
			else{
				offset_x = player->get_x_position() * 3 - SCREEN_WIDTH / 2 / 3 * 3 - OFFSET_TOLERANCE * 3;
			}
		}
	}
	
	if(is_vhold){
		offset_y = map_data->height * 3 / 2 - (SCREEN_HEIGHT - 5) / 2;
	}
	else{
		/*move offset to top*/
		if(player->get_y_position() * 3  < offset_y + (SCREEN_HEIGHT - 5) / 2 - OFFSET_TOLERANCE * 3){
			/*if the player is on the top edge of map*/
			if(player->get_y_position() * 3 - (SCREEN_HEIGHT - 5) / 2 / 3 * 3 + OFFSET_TOLERANCE * 3 < 0){
				offset_y = 0;
			}
			/*if the map can still scroll up*/
			else{
				offset_y = player->get_y_position() * 3 - (SCREEN_HEIGHT - 5) / 2 / 3 * 3 + OFFSET_TOLERANCE * 3;
			}
		}
		/*don't move offset if |offset of player centred| - |current offset| < |block tolerance| * 3*/
		else if((player->get_y_position() * 3 - (SCREEN_HEIGHT - 5) / 2 - offset_y) * (player->get_y_position() * 3 - (SCREEN_HEIGHT - 5) / 2 - offset_y)
			< 9 * OFFSET_TOLERANCE * OFFSET_TOLERANCE){
		}
		/*move offset to bottom*/
		//<---------------------------------------------->
		else{
			if(player->get_y_position() * 3 + (SCREEN_HEIGHT - 5) / 2 - OFFSET_TOLERANCE * 3  > map_data->height * 3){
				offset_y = map_data->height * 3 - (SCREEN_HEIGHT - 5) / 3 * 3;
			}
			else{
				offset_y = player->get_y_position() * 3 - (SCREEN_HEIGHT - 5) / 2 / 3 * 3 - OFFSET_TOLERANCE * 3;
			}
		}
	}
	return 0;
}
//if offset + screen height/2 - tolerance > player position
//<-------O--T<---M----T--|------------------------>
/*get the position of the camera offset*/
int camera_offset_x(){
	return offset_x;
}

int camera_offset_y(){
	return offset_y;
}