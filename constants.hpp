#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 50;
const int FRAMERATE = 3;

/*for camera offset*/
const int OFFSET_TOLERANCE = 5;

#define MAP_DATA_DIR "data/maps/";
#define ITEM_DATA_DIR "data/items.pw"

enum PlayerStance{
	PLAYER_STANCE_UP,
	PLAYER_STANCE_DOWN,
	PLAYER_STANCE_LEFT,
	PLAYER_STANCE_RIGHT,
	PLAYER_STANCE_MOVE_UP,
	PLAYER_STANCE_MOVE_DOWN,
	PLAYER_STANCE_MOVE_LEFT,
	PLAYER_STANCE_MOVE_RIGHT
};

enum ItemType{
	ITEM_TYPE_USE,
	ITEM_TYPE_EQUIP,
	ITEM_TYPE_ETC
};

#endif