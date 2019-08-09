#Ryan's first Makefile! Used to compile the map editor. Type:
#make -f map_editor.mk map_editor
#to build and compile the map editor.

SHELL=cmd
#common variable name for c++ compilers
CXX=g++
CFLAGS=-O3
OBJ = cursor.o key_event.o map_editor.o mapdata.o pw_graphics.o \
	textbox.o uiinfo.o
DEPS = cursor.hpp key_event.hpp mapdata.hpp \
	pw_graphics.h textbox.hpp uiinfo.hpp
SRC = $(DEPS:.hpp=.cpp) $(DEPS:.h=.c) map_editor.cpp

.PHONY: clean
clean:
	del -f *.o map_editor.exe

#target file a.o depends on a.c and header files
%.o: %.cpp $(DEPS)
	$(CXX) $(CFLAGS) -c $< -o $@

%.o: %.c $(DEPS)
	$(CXX) $(CFLAGS) -c $< -o $@

map_editor: $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) -o map_editor.exe
