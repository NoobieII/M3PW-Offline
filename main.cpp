/*program flow of Ryan's game
 *
 *complete this in a short time! Date: March 4, 2019
 *
 *make it so that there is a map - make a simple map - there are solid blocks and portals.
 *the player can move and warp through portals
 *don't use the SDL library this time
 *next time we convert this to use SDL library
 *the time after that, we make it an online online game
 *
 *March 5, 2019 - I had hmm maybe 15 minutes to put this code together.
 *Yes, it will as the above, at 80 characters by 50.
 *When the players walk on a portal, he or she will be teleported to another map.
 *No need for there to be a warp button.
 *
 *
 *March 12, 2019 - I need to know how the game is saved. We should (for now) have a file
 *called save.dat with the map id and position of the player.
 *
 *The map and player instances are in the UI class.
 */
#include "constants.hpp"
#include "error_report.hpp"
#include "item_data_provider.hpp"
#include "pw_graphics.h"
#include "uiinfo.hpp"

#include <SDL/SDL.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]){
	/*times are stored as frame number*/
	int last_render_frame = 0;
	int last_logic_check = 0;
	int delay = 330;
	PWGraphicsContext *screen = NULL;
	
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
		return -1;
	}
	
	freopen("CON", "wt", stdout);
	freopen("CON", "wt", stderr);
	
	if(init_error_log() == -1){
		return -1;
	}
	
	if(UIInfo::get_instance()->init() == -1){
		log_error("Failed to initialize UI");
		return -1;
	}
	
	screen = UIInfo::get_instance()->get_graphics_ctx();
	
	//read any saved data?
	if(ItemDataProvider::get_instance()->load_data()){
		return -1;
	}
	
	while(UIInfo::get_instance()->is_open()){
		printf("a\n");
		//read input
		//do something based on the input
		UIInfo::get_instance()->handle_input();
		
		//logic for the rest of the game world
		while(last_logic_check < /*SDL_GetTicks() * FRAMERATE / 1000*/ PWG_current_frame(screen)){
			//logic check function(s)
			UIInfo::get_instance()->pass_tick(1);
			
			last_logic_check++;
		}
		printf("\n");
		//draw and render
		if(/*SDL_GetTicks() * FRAMERATE / 1000*/PWG_current_frame(screen) > last_render_frame){
			UIInfo::get_instance()->draw();
			
			PWG_render(screen);
			last_render_frame = PWG_current_frame(screen);
		}
		
		delay = (last_render_frame + 1) * /*1000*/CLOCKS_PER_SEC / 3 - /*SDL_GetTicks()*/clock();
		if(delay < 0){
			printf("delay < 0 - Debugging needed!\n");
			delay = 330;
		}
		SDL_Delay(delay);
	}
	
	close_error_log();
	SDL_Quit();
	
	return 0;
}