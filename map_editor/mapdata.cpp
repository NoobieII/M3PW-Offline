#include "mapdata.hpp"
#include <stdio.h>
#include <string.h>

MapData::MapData():
	id(0),
	width(0),
	height(0),
	blocks(),
	portals()
{	
}

MapData::~MapData(){
}

/*returns 0 on success, -1 on failure. Writes a maximum of n bytes*/
int MapData::write_to_bytes(const char *file, size_t n){
	size_t file_size = 0;
	size_t buf;
	int i;
	char file_name[56];
	FILE *out;
	
	/*files have this format:
	 *size(bytes)    purpose
	 *-------------------------
	 *4              absolute size of file, unsigned int, value 64 + x
	 *4              file format tag. 0x8000 is for map data
	 *56             file name string
	 *x              actual file contents
	 */
	
	file_size += 2 * sizeof(int);						/*file size, file format tag*/
	file_size += 56;									/*file name*/
	
	file_size += 3 * sizeof(int);						/*id_, width_, height_*/
	file_size += sizeof(int);							/*number of blocks_*/
	file_size += blocks.size() * sizeof(MapBlock);		/*blocks_*/
	file_size += sizeof(int);							/*number of portals_*/
	file_size += portals.size() * sizeof(MapPortal);	/*portals_*/
	
	if(file_size > n){
		return -1;
	}
	
	out = fopen(file, "wb");
	if(!out){
		return -1;
	}
	
	/*standard file attributes that I use. 0x8000 is the number for mapdata*/
	fwrite(&file_size, sizeof(size_t), 1, out);
	buf = 0x8000;
	fwrite(&buf, sizeof(size_t), 1, out);
	strncpy(file_name, file, 56);
	fwrite(file_name, 56, 1, out);
	
	fwrite(&id, sizeof(int), 1, out);
	fwrite(&width, sizeof(int), 1, out);
	fwrite(&height, sizeof(int), 1, out);
	buf = blocks.size();
	fwrite(&buf, sizeof(size_t), 1, out);
	for(i = 0; i < blocks.size(); ++i){
		fwrite(&(blocks[i]), sizeof(MapBlock), 1, out);
	}
	buf = portals.size();
	fwrite(&buf, sizeof(size_t), 1, out);
	for(i = 0; i < portals.size(); ++i){
		fwrite(&(portals[i]), sizeof(MapPortal), 1, out);
	}
	
	fclose(out);
	return 0;
}

int MapData::read_from_bytes(const char *file, size_t n){
	char buf[56];
	size_t num_blocks, num_portals;
	int i;
	FILE *in;
	
	in = fopen(file, "rb");
	if(!in){
		return -1;
	}
	
	blocks.clear();
	portals.clear();
	
	/*read file info in this format*/
	fread(buf, sizeof(size_t), 1, in);
	if(*(size_t*)(buf) > n){
		fclose(in);
		return -1;
	}
	fread(buf, sizeof(size_t), 1, in);
	if(*(size_t*)(buf) != 0x8000){
		fclose(in);
		return -1;
	}
	fread(buf, 56, 1, in);
	
	/*read mapdata*/
	fread(&id, sizeof(int), 1, in);
	fread(&width, sizeof(int), 1, in);
	fread(&height, sizeof(int), 1, in);
	fread(&num_blocks, sizeof(size_t), 1, in);
	for(i = 0; i < num_blocks; ++i){
		fread(buf, sizeof(MapBlock), 1, in);
		blocks.push_back(*(MapBlock*)(buf));
	}
	fread(&num_portals, sizeof(size_t), 1, in);
	for(i = 0; i < num_portals; ++i){
		fread(buf, sizeof(MapPortal), 1, in);
		portals.push_back(*(MapPortal*)(buf));
	}
	
	fclose(in);
	return 0;
}