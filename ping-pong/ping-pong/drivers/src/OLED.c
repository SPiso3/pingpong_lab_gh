#include <stdbool.h>
#include "../include/XMEM.h"
#include "../include/OLED.h"
#include "../../misc/fonts.h"

//printf redirection
FILE *oled_output;
bool OLED_INVERTED = false;

void OLED_init(){
	OLED_write_cmd(0xAE);    //OFF
	
	//!!! DONT TOUCH THESE SETTINGS !!! --------- (from datasheet)
	OLED_write_cmd(0xa1);    //segment remap
	OLED_write_cmd(0xda);    //common pads hardware: alternative
	OLED_write_cmd(0x12);
	OLED_write_cmd(0xc8);    //common output scan direction:com63~com0
	OLED_write_cmd(0xa8);    //multiplex ration mode:63
	OLED_write_cmd(0x3f);
	OLED_write_cmd(0xd5);    //display divide ratio/osc. freq. mode
	OLED_write_cmd(0x80);
	OLED_write_cmd(0xd9);	 //set pre-charge period
	OLED_write_cmd(0x21);
	OLED_write_cmd(0xdb);    //VCOM deselect level mode
	OLED_write_cmd(0x30);
	OLED_write_cmd(0xad);    //master configuration
	OLED_write_cmd(0x00);
	//-------------------------------------------	
	
	OLED_write_cmd(0xD3);	//set display offset
	OLED_write_cmd(0x00);	
	OLED_write_cmd(0x40);
	
	OLED_write_cmd(0x20);   //Set Memory Addressing Mode
	OLED_write_cmd(0b00);	//PAGE addressing mode => HORIZ -- all auto
	
	OLED_write_cmd(0x81);   //contrast control
	OLED_write_cmd(0xFF);	//contr lvl : 0-255
	
	OLED_write_cmd(0xA6);    //A6 = normal B&W (A7 = inverse W&B)
	OLED_write_cmd(0xA4);    //resume GDDRAM content (A5 = blank screen ON)
	OLED_write_cmd(0xAF);    //ON
	
	oled_output = fdevopen(OLED_putchar8, NULL);
	OLED_reset();
}

//--positioning

void OLED_goto_row(uint8_t row){
	OLED_write_cmd(0xB0 + row);
}
void OLED_goto_col(uint8_t col){
	OLED_write_cmd(col & 0x0F);			// Set low nibble col address : 00-0F
	OLED_write_cmd(0x10 | (col >> 4));	// Set high nibble col address : 10-1F
}
void OLED_goto_pos(uint8_t row, uint8_t col) {
	OLED_goto_row(row);
	OLED_goto_col(col);
}
oled_pos_t OLED_get_addr(uint8_t x, uint8_t y) {
	uint8_t col = x;
	uint8_t row = y/8;
	uint8_t bit = y%8;
	return (oled_pos_t){col, row, bit};
}

//--clearing

void OLED_clear_row(uint8_t row) {
	OLED_goto_pos(row,0);
	for (uint8_t col = 0; col < 128; col++) {
		OLED_write_data(0x00);
	}
}
void OLED_clear(){
	//OLED_restrict_cols(0,127);
	//OLED_restrict_pages(0,7);
	for (uint8_t page = 0; page < 8; page++) {
		OLED_clear_row(page);
	}
}

//--utilities

void OLED_reset() {
	OLED_clear();
	OLED_restrict_cols(0,127);
	OLED_restrict_pages(0,7);
	OLED_goto_pos(0,0);
}

void OLED_invert() {
	OLED_INVERTED ? OLED_write_cmd(0xA6) : OLED_write_cmd(0xA7); //normal : inverted
	OLED_INVERTED = !OLED_INVERTED;
}

void OLED_restrict_cols(uint8_t init_col, uint8_t end_col) {
	OLED_write_cmd(0x21);
	OLED_write_cmd(init_col);
	OLED_write_cmd(end_col);
}

void OLED_restrict_pages(uint8_t init_page, uint8_t end_page) {
	OLED_write_cmd(0x22);
	OLED_write_cmd(init_page);
	OLED_write_cmd(end_page);
}

//----

void OLED_print_8char(char c){
	c = c - 32;
	for (uint8_t i=0; i<8; i++){
		unsigned char byte = pgm_read_byte(&font8[(uint8_t)c][i]);
		OLED_write_data((uint8_t)byte);
	}
}
int OLED_putchar8(char c, FILE *stream){
	OLED_print_8char(c);
	return 0;
}

void OLED_print_figure(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
	OLED_restrict_cols(x,x+width-1);
	OLED_restrict_pages(y,y+height/8-1);
	for(int i=0; i<width*height/8; i++){
		unsigned char byte = pgm_read_byte(&joystick_img[i]);
		OLED_write_data(byte);
	}
}

//--private

void OLED_write_cmd(uint8_t cmd){
	volatile char *ext_mem = (char *) OLED_CMD_ADDR;
	ext_mem[0x0000] = cmd;
}
void OLED_write_data(uint8_t data){
	volatile char *ext_mem = (char *) OLED_DATA_ADDR;
	ext_mem[0x0000] = data;
}