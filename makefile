#Makefile for Ryan's quick code thing

shell = cmd
CXX = g++
CC = g++
CPPFLAGS = -O3 -Wall
CFLAGS = -O3 -Wall
LDLIBS = -lmingw32 -lSDLmain -lSDL.dll
OBJ = camera_offset.o chat_log.o cursor.o draw_handler.o error_report.o \
	input_handler.o inventory.o item.o item_data_provider.o key_event.o main.o map.o \
	mapdata.o option_cursor.o player.o pw_graphics.o pwfile.o textbox.o uiinfo.o

main: $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) -o main.exe $(LDLIBS)

#%.o: %.cpp
#	$(CXX) $(CFLAGS) -c $< -o $@
#pw_graphics.o: pw_graphics.c

.PHONY: clean	
clean:
	del -f *.o main.exe
