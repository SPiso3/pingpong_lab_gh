#include "misc/utils.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define JOY_SEND_TRESHOLD 2

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
	
	while(1){
		_delay_ms(16);
		pos_t pos = JOY_get_rel_pos();
		int8_t x = (int8_t)pos.x;
		int8_t y  = (int8_t)pos.y;
		
		//send only if difference is meaningful to avoid annoying flickering
		if (abs(last_x-x) >= JOY_SEND_TRESHOLD){
			message_t msg = {0xAA, 2, .signed_data={x,y}};
			CAN_send(&msg);
		}
	}
}

ISR(INT0_vect) {
	message_t rec = CAN_receive();
	if (rec.id==CAN_ID_GOAL){
		//OLED_clear_row(0);
		OLED_goto_row(0);
		fprintf(oled_output, "SCORE: %d", score++);
	}
	else if (rec.id==CAN_ID_PWM){
		OLED_clear_row(1);
		OLED_goto_row(1);
		fprintf(oled_output, "pwm: %d", rec.unsigned_data[0]);
	}
	
	//clear interrupt flag
	MCP_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
}