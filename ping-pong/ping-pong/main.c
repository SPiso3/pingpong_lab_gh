#include "misc/utils.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define JOY_SEND_TRESHOLD 5
#define SLIDER_SEND_TRESHOLD 10
extern FILE *oled_output;
uint8_t score = 0;

int main(void) {
	UART_init(MYUBRR);
	XMEM_init();
	ADC_init();
	JOY_init();
	CAN_init();
	OLED_init();
	OLED_reset();
	fprintf(oled_output,"SCORE: 0");
	
	int8_t last_x = 0;
	int8_t last_y = 0;
	uint8_t last_sl = 0;
	uint8_t last_sr = 0;
	uint8_t last_btn = 1;
	
	bool send = false;
	
	while(1){
		_delay_ms(16);
		pos_t pos = JOY_get_rel_pos();
		sliders_t sliders = JOY_get_sliders();
		
		int8_t x = (int8_t)pos.x;
		int8_t y  = (int8_t)pos.y;
		uint8_t sl = sliders.left;
		uint8_t sr = sliders.right;
		uint8_t btn = ((PIND & (1<<JOY_BUTTON))>>JOY_BUTTON);
		
		OLED_clear_row(3);
		OLED_clear_row(4);
		OLED_goto_pos(3,0);
		fprintf(oled_output, "%d %d S: %d %d BTN: %d", x, y, sl, sr, btn);
		
		//send only if difference is meaningful to avoid annoying flickering
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
		
		if (send){
			message_t msg = {CAN_ID_JOYSTICK, 5, .signed_data={last_x,last_y}};
			msg.unsigned_data[2] = last_sl;
			msg.unsigned_data[3] = last_sr;
			msg.unsigned_data[4] = last_btn;
			CAN_send(&msg);
			
			send = false;
		}
		
	}
}

ISR(INT0_vect) {
	message_t rec = CAN_receive();
	if (rec.id==CAN_ID_GOAL){
		//OLED_clear_row(0);
		OLED_goto_pos(0,0);
		fprintf(oled_output, "SCORE: %d", score++);
		OLED_goto_pos(1,0);
		fprintf(oled_output, "ADC: %d%d", rec.unsigned_data[0], rec.unsigned_data[1]);
	}
	else if (rec.id==CAN_ID_PWM){
		OLED_clear_row(2);
		OLED_goto_pos(2,0);
		fprintf(oled_output, "pwm: %d", rec.unsigned_data[0]);
	}
	
	//clear interrupt flag
	MCP_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
}