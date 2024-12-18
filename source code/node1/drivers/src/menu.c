#include "../include/menu.h"

extern FILE *oled_output;

void menu_init(){
	menu_t menu_null = {"root",NULL,NULL,{NULL},0};
	
	menu_add(&menu_null, "NEW GAME", &new_game);
	
	menu_ptr menu_settings = menu_add(&menu_null, "SETTINGS", NULL);
	menu_add(menu_settings, "x-y", &settings_mode1);
	menu_add(menu_settings, "sl-sr", &settings_mode2);
	menu_add(menu_settings, "sr-x", &settings_mode3);
	menu_add(menu_settings, "x-x", &settings_mode4);
	menu_add(menu_settings, "sr-sr", &settings_mode5);
	
	menu_ptr menu_test = menu_add(&menu_null, "TEST", NULL);
	menu_add(menu_test, "SRAM", &test_SRAM);
	menu_add(menu_test, "OLED", &test_OLED);
	
	/*
	TO BE ADDED IF MEMSPACE is enough
	
	menu_add(menu_test, "UART", &test_UART);
	menu_add(menu_test, "SRAM", &test_SRAM);
	menu_add(menu_test, "XMEM", &test_XMEM);
	menu_add(menu_test, "ADC", &test_ADC);
	menu_add(menu_test, "JOYSTICK", &test_JOYSTICK);
	menu_add(menu_test, "OLED", &test_OLED);
	menu_add(menu_test, "MCP", &test_MCP_loopback);
	*/
	
	menu_ptr current = &menu_null;
	menu_loop(current);
}

void menu_loop(menu_ptr current){
	uint8_t selected_subM = 0;
	while(1){
		_delay_ms(100);
		//printf("%s\n\r",current->text);
		dir direction = JOY_get_dir();
		switch(direction){
			case UP:
				if (selected_subM>0)
					selected_subM--;
				else
					selected_subM = 0;
				break;
			case DOWN:
				if (selected_subM<(current->subM_n)-1)
					selected_subM++;
				else
					selected_subM = (current->subM_n)-1;
				break;
			case LEFT:
				if(current->parent != NULL){
					current = current->parent;
					selected_subM = 0;
				}
				break;
			case RIGHT:
				if(current->subMenu[selected_subM]->function != NULL)
					current->subMenu[selected_subM]->function();
				else{
					if(current->subMenu[selected_subM]->subMenu[0] != NULL){
						current = current->subMenu[selected_subM];
						selected_subM = 0;
					}
				}
				break;
			default:
				break;
		}
		
		OLED_reset();
		display_menu(current, selected_subM);
	}
	
}

//--private

void display_menu(menu_ptr m, uint8_t arrow_pos){
	OLED_clear();
	
	OLED_goto_pos(arrow_pos, 0);
	fprintf(oled_output,"> ");
	//printf("%u\n\r", arrow_pos);
	
	for(uint8_t i = 0; i < m->subM_n; i++){
		OLED_goto_pos(i,16);
		fprintf(oled_output,"%s",m->subMenu[i]->text);
	}
}

menu_ptr menu_add(menu_ptr parent, char * text, void (*function)()) {
	//printf("size:%d",sizeof(menu_t));
	menu_ptr subMenu = (menu_ptr)malloc(sizeof(menu_t));
	subMenu->text = text;
	subMenu->function = function;
	subMenu->parent = parent;
	subMenu->subM_n = 0;
	
	if (parent != NULL){
		parent->subMenu[parent->subM_n++] = subMenu;
	}
	return subMenu;
}