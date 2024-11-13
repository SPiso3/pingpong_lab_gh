#include "misc/utils.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define JOY_SEND_TRESHOLD 4

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
	uint8_t last_sl = 0;
	
	while(1){
		_delay_ms(16);
		pos_t pos = JOY_get_rel_pos();
		sliders_t sliders = JOY_get_sliders();
		
		int8_t x = (int8_t)pos.x;
		int8_t y  = (int8_t)pos.y;
		uint8_t sl = sliders.left;
		
		//send only if difference is meaningful to avoid annoying flickering
		if (abs(x-last_x) >= JOY_SEND_TRESHOLD || abs(sl-last_sl) >= JOY_SEND_TRESHOLD){
			last_x = x;
			last_sl = sl;
			message_t msg = {CAN_ID_JOYSTICK, 3, .signed_data={x,y}};
			msg.unsigned_data[2] = sl;
			CAN_send(&msg);
			printf("%d -> %d\n\r", sl, last_sl);
		}
	}
}

ISR(INT0_vect) {
	message_t rec = CAN_receive();
	if (rec.id==CAN_ID_GOAL){
		//OLED_clear_row(0);
		OLED_goto_pos(0,0);
		fprintf(oled_output, "SCORE: %d", score++);
	}
	else if (rec.id==CAN_ID_PWM){
		OLED_clear_row(1);
		OLED_goto_pos(1,0);
		fprintf(oled_output, "pwm: %d", rec.unsigned_data[0]);
	}
	
	//clear interrupt flag
	MCP_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
}