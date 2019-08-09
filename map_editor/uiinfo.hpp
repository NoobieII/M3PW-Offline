/*March 16, 2019
 *In the map editor, the initialization takes a filename or a null string for
 *the map file to load initially. If NULL, the editor will open a new map.
 *If not null, the map editor will open a file with the given name and will open
 *the map in the editor. If the file cannot be opened, the UI will close.
 *
 *In the map editor, the user can use the arrow keys to move the cursor and place
 *map elements down. TAB switches the element. The current element type to be placed will
 *only be visible. DEL removes any element on the cursor.
 *ENT places the element, if none of the same elements are on the cursor already. ' '
 *edits the properties of the element.
 *
 *When editing the properties of an element, TAB switches the text field. The portal properties
 *are typed in the text field and pressing ENT confirms the value. Pressing any arrow
 *key goes out of the element editor.
 *
 */

#ifndef UIINFO_HPP
#define UIINFO_HPP

#include "cursor.hpp"
#include "mapdata.hpp"
#include "textbox.hpp"

struct PWGraphicsContext;

class UIInfo{
public:
	UIInfo();
	~UIInfo();
	
	int initialize(const char *filename);
	void handle_input();
	void handle_input_map_properties();
	void handle_input_editor();
	void handle_input_element_portal();
	void handle_input_menu();
	void handle_input_type_save_filename();
	void handle_input_type_load_filename();
	
	void draw();
	void draw_map_properties();
	void draw_editor();
	void draw_element_portal();
	void draw_menu();
	void draw_type_save_filename();
	void draw_type_load_filename();
	
	int is_open();
	
private:
	int state_;
	int is_open_;
	
	/*map properties editor (appears when creating a new file)*/
	Cursor map_properties_cursor_;
	TextBox id_textbox_;
	TextBox width_textbox_;
	TextBox height_textbox_;
	
	/*map editor*/
	int element_type_; /*0 for block, 1 for portal*/
	Cursor cursor_x_;
	Cursor cursor_y_;
	int block_id_;
	int portal_id_;
	int offset_x_;		/*if the map is larger than the screen, use an offset*/
	int offset_y_;
	
	/*map element editor*/
	/*int element_type_*/
	Cursor selected_text_;
	int block_edit_index_; /*id of selected block*/
	TextBox portal_dest_map_id_textbox_;
	TextBox portal_dest_portal_id_textbox_;
	int portal_edit_index_; /*id of selected portal*/
	
	Cursor menu_cursor_;
	TextBox load_filename_;
	TextBox save_filename_;
	
	MapData map_data_;
	PWGraphicsContext *graphics_ctx_;
	//std::map<int, PWSprite*> block_sprites_;
};

/*UI_STATE_MAP_PROPERTIES
 *
 *ID:     ___
 *WIDTH:  ___
 *HEIGHT: ___
 *
 *UI_STATE_EDITOR
 *on the top of the screen, display...
 *Map id=0 width=0 height =0
 *
 * .  +  |
 *. .+++\|/
 * .  + - -
 * 1  2  3
 *Figure 1. Element types. 1 is the cursor, 2 is block, 3 is portal.
 *
 *on the bottom of the screen, display...
 *Portal id=0 dest_map=0 dest_portal=0
 *
 *UI_STATE_ELEMENT_PORTAL
 *
 *dest_map:    ____
 *dest_portal: ____
 *
 *UI_STATE_MENU
 *
 *>Save File
 * Load File
 * Quit
 *
 *UI_STATE_TYPE_SAVE_FILENAME - similar to loading
 *
 *Save to: _______
 */

enum UIState{
	UI_STATE_MAP_PROPERTIES,
	UI_STATE_EDITOR,
	UI_STATE_ELEMENT_PORTAL,
	UI_STATE_MENU,
	UI_STATE_TYPE_SAVE_FILENAME,
	UI_STATE_TYPE_LOAD_FILENAME
};

#endif