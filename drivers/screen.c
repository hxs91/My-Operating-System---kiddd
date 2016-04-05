#include "screen.h"
#include "type.h"
#include "const.h"

extern void memory_copy(char * source, char * dest, int num_bytes);

/*
get the offset according the given col and row.
notice that, every column occupies 2 bytes
*/
int get_screen_offset(int col, int row) {
	return (row * MAX_COLS + col) * 2;
}

/*
disable the display of the cursor
*/
void disable_cursor() {
	port_byte_out(REG_SCREEN_CTRL, 0x0A);
	u8 cursor_start = port_byte_in(REG_SCREEN_DATA) & 0x1F;	//get cursor scanline start

	port_byte_out(REG_SCREEN_CTRL, 0x0A);
	port_byte_out(REG_SCREEN_DATA, cursor_start | 0x20);	//set enable bit, 1 for disable
}

/*
enable the display of the cursor
*/
void enable_cursor() {
	port_byte_out(REG_SCREEN_CTRL, 0x0A);
	u8 cursor_start = port_byte_in(REG_SCREEN_DATA) & 0x1F;	//get cursor scanline start and set enable bit

	port_byte_out(REG_SCREEN_CTRL, 0x0A);
	port_byte_out(REG_SCREEN_DATA, cursor_start);
}

/*
get the offset of current cursor
*/
int get_cursor() {
	u16 offset = 0;

	// get the value of Cursor Location High Register
	port_byte_out(REG_SCREEN_CTRL, 0x0E);
	offset |= port_byte_in(REG_SCREEN_DATA);
	
	offset <<= 8;
	// get the value of Cursor Location Low Register
	port_byte_out(REG_SCREEN_CTRL, 0x0F);
	offset |= port_byte_in(REG_SCREEN_DATA);
	return offset * 2;
}

/*
update the cursor
*/
void set_cursor(int offset) {
	u16 cursor_location = offset / 2;

	//first set the Cursor Location Low Register
	port_byte_out(REG_SCREEN_CTRL, 0x0F);
	port_byte_out(REG_SCREEN_DATA, (u8) (cursor_location & 0x00FF));
	cursor_location >>= 8;
	//then set the Cursor Location High Register
	port_byte_out(REG_SCREEN_CTRL, 0x0E);
	port_byte_out(REG_SCREEN_DATA, (u8) (cursor_location & 0x00FF));
}

/*
Advance the text cursor, scrolling the video buffer if necessary.
*/
int handle_scrolling(int cursor_offset) {
	if (cursor_offset < (MAX_COLS*MAX_ROWS*2)) {
		return cursor_offset;
	}
	//move each line up
	int i;
	for (i = 1; i < MAX_ROWS; i++) {
		memory_copy((char *)(get_screen_offset(0, i) + VIDEO_ADDRESS),
					(char *)(get_screen_offset(0, i-1) + VIDEO_ADDRESS),
					MAX_COLS * 2);
	}
	//clean up the last row
	char * last_line = (char *)(get_screen_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS);
	for (i = 0; i < MAX_COLS * 2 ; i++ ) {
		last_line[i] = ' ';
	}
	//reset the cursor_offset, thus it will appear at the bottom of the screen
	cursor_offset -= 2 * MAX_COLS;
	return cursor_offset;
}

/*
target: the character we want to print
col, row: the position where we want to print, it also indicates the cursor
attribute: atrribute byte which can control the colour of charactor
*/
void print_char(char target, int col, int row, char attribute) {
	char * vidmem = (char *) VIDEO_ADDRESS;
	
	// if attribute is 0, then WHITE_ON_BLACK default
	if (!attribute) {
		attribute = WHITE_ON_BLACK;
	}

	int offset;
	if (col >= 0 && row >= 0) {
	//if col and row are valid, the using them to get its corresponding offset
		offset = get_screen_offset(col, row);
	}
	else{
	// otherwise, get the offset of current cursor
		offset = get_cursor();
	}

	if (target == '\n') {
		// TODO: why 2 times MAX_COLS?
		// since one col need 2 bytes!!!
		int rows = offset / (2 * MAX_COLS);
		offset = get_screen_offset(79, rows);
	}
	else{
		vidmem[offset] = target;
		vidmem[offset+1] = attribute;
	}

	//move the cursor ahead
	offset += 2;
	// when we reach the bottom of the screen, we need to scroll the screen
	//offset = handle_scrolling(offset);
	// update the cursor position
	set_cursor(offset);
}

/*
print a string at a specific position
*/
void print_at(char * str, int col, int row) {
	if (col >= 0 && row >= 0) {
	//set the begining of the string
		set_cursor(get_screen_offset(col, row));
	}
	int i = 0;
	while (str[i] != 0) {
		print_char(str[i++], -1, -1, WHITE_ON_BLACK);
	}
}

/*
print a string started from the current cursor
*/
void print(char * str) {
	print_at(str, -1, -1);
}

/*
clear all the content on the screen
*/
void clear_screen() {
	int row = 0;
	int col = 0;
	//clear
	for (row = 0; row < MAX_ROWS; row ++) {
		for (col = 0; col < MAX_COLS; col ++) {
			print_char(' ', col, row, WHITE_ON_BLACK);
		}
	}

	//then reset the cursor
	set_cursor(get_screen_offset(0, 0));
}

PUBLIC void print_hex(int num) {
	char output[16];
	itoa(output, num);
	print(output);
}