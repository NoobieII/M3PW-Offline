/*map editor*/
#include "uiinfo.hpp"
#include <conio.h>

int main(int argc, char *argv[]){
	UIInfo ui;
	const char *filename = NULL;
	
	if(argc > 1){
		filename = argv[1];
	}
	if(ui.initialize(filename) == -1){
		return -1;
	}
	
	while(ui.is_open()){
		ui.draw();
		
		while(!kbhit());
		
		ui.handle_input();
	}
	
	return 0;
}