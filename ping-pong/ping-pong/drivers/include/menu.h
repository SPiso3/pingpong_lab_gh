#ifndef MENU_H_
#define MENU_H_

#include "../include/gamepad.h"
#include "../include/OLED.h"
#include "../include/UART.h"
#include "../../misc/utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>


#define MAX_SUBMENUS 10
 
typedef struct Menu {
	char * text; 
	void (*function)(); 
	struct Menu * parent; 
	struct Menu * subMenu[MAX_SUBMENUS];
	uint8_t subM_n;
} menu_t, *menu_ptr;


void menu_init();
void menu_mainf(menu_ptr current);
void update_menu(menu_ptr m, uint8_t arrow_pos);
menu_ptr menu_add(menu_ptr parent, char * text, void (*function)());
#endif /* MENU_H_ */