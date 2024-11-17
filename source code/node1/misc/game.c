#include "game.h"

//global variables
extern FILE *oled_output;
uint8_t lives = MAXLIVES;
bool gameover = false;

//time survived (not super-precise but do his job)
uint16_t ms;
uint8_t sec, min;

//---------------------------------------------------

void restart_timer(){
	//restart the timer
	ms = 0;
	sec = 0;
	min = 0;
}

void update_timer(){
	//update time
	ms += GAME_SPEED;
	if(ms>=1000){
		ms = ms%1000;
		sec++;
	}
	if(sec>=60){
		sec = sec%60;
		min++;
	}
	//idk if you can reach this but.. who knows?
	if(min>=60){
		min = min%60;
	}
}

void gameloop(){
	
	restart_timer();
	
	//to handle noise flickering
	int8_t last_x = 0;
	int8_t last_y = 0;
	uint8_t last_sl = 0;
	uint8_t last_sr = 0;
	uint8_t last_btn = 1;
	
	bool send = false;
	
	//start setting up the screen
	OLED_clear();
	OLED_goto_pos(0,0);
	fprintf(oled_output, "LIVES: %d", lives);
	
	while(1 && !gameover){
		_delay_ms(GAME_SPEED);
		
		//samples from joystick
		pos_t pos = JOY_get_rel_pos();
		sliders_t sliders = JOY_get_sliders();
		
		//extract single variables
		int8_t x = (int8_t)pos.x;
		int8_t y  = (int8_t)pos.y;
		uint8_t sl = sliders.left;
		uint8_t sr = sliders.right;
		uint8_t btn = ((PIND & (1<<JOY_BUTTON))>>JOY_BUTTON);
		
		//update only if difference is meaningful to avoid annoying flickering
		//send only if one of these has changed
		if (abs(x-last_x) >= JOY_SEND_TRESHOLD){
			last_x = x;
			send = true;
		}
		if (abs(y-last_y) >= JOY_SEND_TRESHOLD){
			last_y = y;
			send = true;
		}
		if (abs(sl-last_sl) >= SLIDER_SEND_TRESHOLD){
			last_sl = sl;
			send = true;
		}
		if (abs(sr-last_sr) >= SLIDER_SEND_TRESHOLD){
			last_sr = sr;
			send = true;
		}
		if (last_btn != btn){
			last_btn = btn;
			send = true;
		}
		
		//send only if one of those has changed
		if (send){
			message_t msg = {CAN_ID_JOYSTICK, 5, .signed_data={last_x,last_y}};
			msg.unsigned_data[2] = last_sl;
			msg.unsigned_data[3] = last_sr;
			msg.unsigned_data[4] = last_btn;
			CAN_send(&msg);
			
			send = false; //job done, back to sleep
		}
		
		update_timer();
		
		OLED_clear_row(1);
		OLED_goto_pos(1,0);
		fprintf(oled_output, "TIME: %2d:%2d", min, sec);
		
		OLED_clear_row(3);
		OLED_clear_row(4);
		OLED_goto_pos(3,0);
		fprintf(oled_output, "%d %d S: %d %d BTN: %d", x, y, sl, sr, btn);
		
		if(lives<=0){
			gameover = true;
			lives = 3;
			printf("GAMEOVER\n\r");
			OLED_clear();
			OLED_goto_pos(3,0);
			fprintf(oled_output, "GAMEOVER!");
			message_t msg = {.id = CAN_ID_GAMEOVER, .length=1, .signed_data={0}};
			CAN_send(&msg);
			_delay_ms(2000);
		}
	}
}

ISR(INT0_vect) {
	message_t rec = CAN_receive();
	
	//update lives (also on screen)
	if (rec.id==CAN_ID_GOAL){
		OLED_clear_row(0);
		OLED_goto_pos(0,0);
		fprintf(oled_output, "LIVES: %d", lives--);
	}
	
	//clear interrupt flag
	MCP_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
}

//---------------------------------------------------
//messages to node2

void settings(uint8_t mode){
	message_t msg = {.id = CAN_ID_SETTINGS, .length=1, .signed_data={mode}};
	CAN_send(&msg);
	printf("SETTINGS send with MODE: %d\n\r", mode);
	
	//screen blinks to reply
	OLED_invert();
	_delay_ms(100);
	OLED_invert();
}

//five controller modes available
void settings_mode1(){ settings(MODE_1); };
void settings_mode2(){ settings(MODE_2); };
void settings_mode3(){ settings(MODE_3); };
void settings_mode4(){ settings(MODE_4); };
void settings_mode5(){ settings(MODE_5); };

//new game command
void new_game(){
	gameover = false;
	
	//send can message to node2
	message_t msg = {.id = CAN_ID_NEW_GAME, .length=1, .signed_data={0}};
	CAN_send(&msg);
	printf("NEW GAME\n\r");
	
	OLED_clear();
	OLED_goto_row(3);
	printf("CALIBRATION...\n\r");
	fprintf(oled_output, "CALIBRATION...");
	_delay_ms(CALIB_TIME); //waiting the calibration time
	
	//start the game
	printf("gameloop()\n\r");
	gameloop();
}































//UTILS -----------------------------------------------------------------------------

extern FILE *oled_output;

void test_SRAM(void)
{	
	XMEM_init();
	UART_init(MYUBRR);
	volatile char *ext_ram = (char *) SRAM_BASE_ADDR; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n\r");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			//printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			//printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n\r%4d errors in write phase and \n\r%4d errors in retrieval phase\n\r\n\r", write_errors, retrieval_errors);
}

void test_OLED(){
	UART_init(MYUBRR);
	XMEM_init();
	OLED_init();
	OLED_reset();
	
	for(int p=0; p<8; p++){
		OLED_goto_pos(p,p*8);
		for(int c=0; c<8; c++){
			OLED_write_data(0xFF);
			_delay_ms(50);
		}
	}
	
	OLED_goto_pos(7,128-8);
	for(int c=0; c<8; c++)
	OLED_write_data(0xFF);
	_delay_ms(100);
	
	OLED_goto_pos(0,128-8);
	for(int c=0; c<8; c++)
	OLED_write_data(0xFF);
	_delay_ms(100);
	
	for(uint8_t i=0; i<4; i++){
		_delay_ms(200);
		OLED_invert();
	}
	
	OLED_reset();
	fprintf(oled_output, "prova ...");
	_delay_ms(1000);
	OLED_reset();
	
	OLED_reset();
	char value = ' ';
	while(value < ' ' + 95){
		fprintf(oled_output, "%c",value++);
		_delay_ms(50);
	}
}