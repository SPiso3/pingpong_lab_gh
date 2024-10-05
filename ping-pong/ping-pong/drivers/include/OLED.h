#ifndef OLED_H_
#define OLED_H_

#include <avr/io.h>
#include "../include/XMEM.h"
#include "../include/OLED.h"

#define OLED_CMD_ADDR 0x1000
#define OLED_DATA_ADDR 0x1200

void OLED_init();
void OLED_goto_row(uint8_t row);
void OLED_goto_col(uint8_t col);
void oled_goto_pos(uint8_t row, uint8_t col);

void OLED_clear_row(uint8_t row);
void OLED_clear();

void OLED_reset();
void OLED_print_8char(unsigned char c);

void OLED_write_cmd(uint8_t cmd);
void OLED_write_data(uint8_t data);

#endif /* OLED_H_ */