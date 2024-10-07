#ifndef OLED_H_
#define OLED_H_

#include <stdio.h>
#include <avr/io.h>
#include <string.h>
#include "../include/XMEM.h"
#include "../include/OLED.h"

typedef struct{
	uint8_t col;
	uint8_t row;
	uint8_t bit;
} oled_pos_t;

#define OLED_CMD_ADDR 0x1000
#define OLED_DATA_ADDR 0x1200

void OLED_init();
void OLED_goto_row(uint8_t row);
void OLED_goto_col(uint8_t col);
void OLED_goto_pos(uint8_t row, uint8_t col);
oled_pos_t OLED_get_addr(uint8_t x, uint8_t y);

void OLED_clear_row(uint8_t row);
void OLED_clear();

void OLED_reset();
void OLED_print_8char(char c);
int OLED_putchar8(char c, FILE *stream);

void OLED_print_figure(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

void OLED_write_cmd(uint8_t cmd);
void OLED_write_data(uint8_t data);

#endif /* OLED_H_ */