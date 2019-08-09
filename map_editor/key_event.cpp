#include "key_event.hpp"

#include <conio.h>
#include <string.h>

/*
	Used to quickly convert special characters such as arrow keys to the enumerated
	values. Keys such as tab, backspace, esc, are in the enumeration to so integer
	literals don't have to be used to represent them.
*/

const unsigned char kConvertSpecialKey[256] = 
{
	0, 0, 0, 0, 0, 0, 0, 0, KeyEvent::BKS, KeyEvent::TAB,	//0
	0, 0, 0, KeyEvent::ENT, 0, 0, 0, 0, 0, 0,	//10
	0, 0, 0, 0, 0, 0, 0, KeyEvent::ESC, 0, 0,	//20
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//30
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//40
	0, 0, 0, 0, 0, 0, 0, 0, 0, KeyEvent::F1,	//50
	KeyEvent::F2, KeyEvent::F3, KeyEvent::F4, KeyEvent::F5, KeyEvent::F6, KeyEvent::F7, KeyEvent::F8, KeyEvent::F9, KeyEvent::F10, 0,	//60
	0, KeyEvent::HOM, KeyEvent::KU, KeyEvent::PUP, 0, KeyEvent::KL, 0, KeyEvent::KR, 0, KeyEvent::END,	//70
	KeyEvent::KD, KeyEvent::PDN, KeyEvent::INS, KeyEvent::DEL, KeyEvent::F1, KeyEvent::F2, KeyEvent::F3, KeyEvent::F4, KeyEvent::F5, KeyEvent::F6,	//80
	KeyEvent::F7, KeyEvent::F8, KeyEvent::F9, KeyEvent::F10, KeyEvent::F1, KeyEvent::F2, KeyEvent::F3, KeyEvent::F4, KeyEvent::F5, KeyEvent::F6,	//90
	KeyEvent::F7, KeyEvent::F8, KeyEvent::F9, KeyEvent::F10, KeyEvent::F1, KeyEvent::F2, KeyEvent::F3, KeyEvent::F4, KeyEvent::F5, KeyEvent::F6,	//100
	KeyEvent::F7, KeyEvent::F8, KeyEvent::F9, KeyEvent::F10, 0, KeyEvent::KL, KeyEvent::KR, 0, KeyEvent::PDN, 0,	//110
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//120
	0, 0, 0, KeyEvent::F11, KeyEvent::F12, KeyEvent::F11, KeyEvent::F12, KeyEvent::F11, KeyEvent::F12, KeyEvent::F11,	//130
	KeyEvent::F12, 0, 0, 0, 0, 0, KeyEvent::INS, KeyEvent::DEL, KeyEvent::TAB, 0,	//140
	0, KeyEvent::HOM, KeyEvent::KU, KeyEvent::PUP, 0, KeyEvent::KL, 0, KeyEvent::KR, 0, KeyEvent::END,	//150
	KeyEvent::KD, KeyEvent::PDN, KeyEvent::INS, KeyEvent::DEL, 0, 0, 0, 0, 0, 0,	//160
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//170
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//180
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//190
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//200
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//210
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//220
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//230
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//240
	0, 0, 0, 0, 0, 0
};

/*
	These values will be stored in the KeyHandler class ch_input_[]
*/
const unsigned char kControlValues[] = 
{
KeyEvent::F1, KeyEvent::F2, KeyEvent::F3, KeyEvent::F4, KeyEvent::F5, KeyEvent::F6, KeyEvent::F7, KeyEvent::F8, KeyEvent::BKS, KeyEvent::TAB,
KeyEvent::F9, KeyEvent::F10, KeyEvent::F11, KeyEvent::ENT, KeyEvent::F12, KeyEvent::HOM, KeyEvent::INS, KeyEvent::DEL, KeyEvent::PUP, KeyEvent::PDN,
KeyEvent::END, KeyEvent::KU, KeyEvent::KL, KeyEvent::KR, KeyEvent::KD, 0, 0, KeyEvent::ESC, 0, 0,
0, 0, ' ', '!', '\"', '#', '$', '%', '&', '\'',	//30
'(', ')', '*', '+', ',', '-', '.', '/', '0', '1',	//40
'2', '3', '4', '5', '6', '7', '8', '9', ':', ';',	//50
'<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E',	//60
'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',	//70
'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',	//80
'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b', 'c',	//90
'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',	//100
'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',	//110
'x', 'y', 'z', '{', '|', '}', '~', 0, 0, 0	//120
};

static KeyEvent key_events, key_events_buf;

KeyEvent *get_key_events(){
	
	while(kbhit()){
		unsigned char ch = getch();
		
		if(ch == 0 || ch == 224){
			ch = getch();
			key_events.keys_pressed_[kConvertSpecialKey[ch] ]++;
		}
		else{
			key_events.keys_pressed_[kControlValues[ch] ]++;
		}
	}
	
	memcpy(&key_events_buf, &key_events, sizeof(KeyEvent));
	memset(&key_events, 0, sizeof(KeyEvent));
	
	return &key_events_buf;
}

char get_char_input(){
	unsigned char ch = getch();
	
	if(ch == 0 || ch == 224){
		ch = getch();
		return kConvertSpecialKey[ch];
	}
	else{
		return (char)(ch);
	}
}