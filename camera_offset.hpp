#ifndef CAMERA_OFFSET_HPP
#define CAMERA_OFFSET_HPP

class MapData;
class Player;

/*some helper functions to keep track of where to render the map.*/

/*call when player is initially put in the map*/
int camera_offset_set(Player *p, MapData *m);

/*call each frame after camera_offset_set() is called*/
int camera_offset_update();

/*get the position of the camera offset*/
int camera_offset_x();
int camera_offset_y();

#endif