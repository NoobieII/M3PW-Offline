#ifndef UIINFO_HPP
#define UIINFO_HPP

/*March 5, 2019
 *
 *the ui consists similarly to what I had in January 2017.
 *The program starts at a selection screen (start screen) with options
 *	>New Game
 *	 Load Game
 *	 Quit
 *
 *New Game creates a file for the game
 *Load Game opens the file and starts the game
 *
 *arrow keys select the option
 *pressing enter executes the option
 *0-new game creates new game 1-loads game 2-quits
 *
 *in the game, pressing ESC goes back to the start screen.
 */

#include "chat_log.hpp"
#include "cursor.hpp"
#include "option_cursor.hpp"
#include "textbox.hpp"

class Inventory;
class Map;
class MapData;
class Player;
struct PWGraphicsContext;
 
class UIInfo{
public:
	static UIInfo *get_instance();
	void handle_input();
	void draw();
	
	/*returns 0 on success, -1 on failure*/
	int init();
	
	int is_open();
	
	void handle_input_start_quit_screen(char input);
	void handle_input_in_game(char input);
	void handle_input_map_transition(char input);
	void handle_input_text_in_game(char input);
	void handle_input_inventory(char input);
	void handle_input_inventory_option(char input);
	
	void draw_start_quit_screen();
	void draw_in_game();
	void draw_map_transition();
	void draw_text_in_game();
	void draw_inventory();
	void draw_inventory_option();
	
	void pass_tick(int t);
	void process_chat_command(const char *str); /*str points to the first character after '/' */
	void populate_item_options(int item_id);  /*populates inventory_option_cursor_ to show possible options for the item selected*/
	
	PWGraphicsContext *get_graphics_ctx();
private:
	UIInfo();
	~UIInfo();
	
	static UIInfo *singleton_;
	
	int state_;
	int is_open_;
	
	/*start screen*/
	Cursor start_screen_cursor_;
	
	/*in game screen*/
	Player *player_;
	Map *map_;
	MapData *map_data_;
	int map_transition_start_time_;
	TextBox chat_box_;
	ChatLog chat_log_;
	
	//inventory screen
	Inventory *inventory_;
	Cursor inventory_item_cursor_;
	OptionCursor inventory_option_cursor_;
	
	PWGraphicsContext *graphics_ctx_;
};

enum state{
	STATE_START_QUIT_SCREEN,
	STATE_IN_GAME,
	STATE_MAP_TRANSITION,
	STATE_TEXT_IN_GAME,
	STATE_INVENTORY,
	STATE_INVENTORY_OPTION
};

enum item_option{
	ITEM_OPTION_CANCEL,
	ITEM_OPTION_USE,
	ITEM_OPTION_SPECIAL,
	ITEM_OPTION_EQUIP
};

/*in STATE_START_QUIT_SCREEN
 *
 *>Start Game
 * Quit Game
 */
 
#endif