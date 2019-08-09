#ifndef MAPDATA_HPP
#define MAPDATA_HPP

#include <vector>
#include <stdlib.h>

/*MapData id's start at 10,000,000 to 19,999,999*/

struct MapBlock{
	int id;
	short x;
	short y;
	/*short width;
	short height;
	int sprite_id;*/
};

struct MapPortal{
	int id;
	short x;
	short y;
	int dest_map_id;
	int dest_portal_id;
};

class MapData{
public:
	MapData();
	~MapData();
	
	/*returns 0 on success, -1 on failure. Writes a maximum of n bytes*/
	int write_to_bytes(const char *file, size_t n);
	int read_from_bytes(const char *file, size_t n);
	
	int id;
	int width;
	int height;
	std::vector<MapBlock> blocks;
	std::vector<MapPortal> portals;
private:
	MapData(const MapData &m);
};

#endif