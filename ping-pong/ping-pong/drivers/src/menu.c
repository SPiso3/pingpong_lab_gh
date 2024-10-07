#include "../include/menu.h"

extern FILE *oled_output;

void menu_init(){
	menu_ptr menu_null = (menu_ptr)malloc(sizeof(menu_t));
	menu_ptr menu_highscore = menu_add(menu_null, "HIGH SCORE", NULL);
	menu_ptr menu_play = menu_add(menu_null, "PLAY", NULL);
	menu_ptr menu_play1 = menu_add(menu_play, "PLAY1", &test_OLED);
	menu_ptr menu_play2 = menu_add(menu_play, "PLAY2", NULL);
	menu_ptr current = menu_null;
	menu_mainf(current);
}

void menu_mainf(menu_ptr current){
	uint8_t selected_subM = 0;
	pos_t center = JOY_calibrate();
	while(1){
		_delay_ms(500);
		OLED_reset();
		update_menu(current, selected_subM);
		pos_t pos = JOY_get_rel_pos(center);
		dir direction = JOY_get_dir(pos);
		switch(direction){
			case UP:
				if (selected_subM=!0) selected_subM--;
				break;
			case DOWN:
				if (selected_subM<(current->subM_n)-1) selected_subM++;
				break;
			case LEFT:
				if(current->parent != NULL){
					current = current->parent;
					selected_subM = 0;
				}
				break;
			case RIGHT:
				if(current->function!= NULL)
					current->function();
				else{
					current = current->subMenu[selected_subM];
					selected_subM = 0;
				}
				break;
			case IDLE:
				break;
		}
	}
	
}

//--private

void update_menu(menu_ptr m, uint8_t arrow_pos){
	uint8_t i = 0;
	OLED_reset();
	while(i < m->subM_n){
		OLED_goto_pos(i,0);
		//fprintf(oled_output,"QUI");
		fprintf(oled_output,"%s",m->subMenu[i]->text);
		i++;
	}
	OLED_goto_pos(arrow_pos, 128-16);
	printf("%u\n\r", (unsigned)arrow_pos);
	fprintf(oled_output,"<-");
}

menu_ptr menu_add(menu_ptr parent, char * text, void (*function)()) {
	menu_ptr subMenu = (menu_ptr)malloc(sizeof(menu_t)); 
	subMenu->text = text;
	subMenu->function = function; 
	subMenu->parent = parent;
	subMenu->subM_n = 0;
	if (parent != NULL){
		int i = 0;
		while (parent->subMenu[i] != NULL) {
			i++;
		}
		// Add subMenu
		parent->subMenu[i] = subMenu;
		parent->subM_n++;
	}
	return subMenu;
}
