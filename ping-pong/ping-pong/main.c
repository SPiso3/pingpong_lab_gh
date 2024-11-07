#include "misc/utils.h"
#include <avr/io.h>
#include <avr/interrupt.h>

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
	
	while(1){
		_delay_ms(16);
		pos_t pos = JOY_get_rel_pos();
		int8_t x = (int8_t)pos.x;
		int8_t y  = (int8_t)pos.y;
		message_t msg = {0xAA, 2, .signed_data={x,y}};
		CAN_send(&msg);
	}
}

ISR(INT0_vect) {
	message_t rec = CAN_receive();
	if (rec.id==CAN_ID_GOAL){
		OLED_clear();
		fprintf(oled_output,"SCORE: %d",score++);
	}
	else if (rec.id==CAN_ID_PWM){
		printf("pwm: %d\n\r",rec.unsigned_data[0]);
	}
	MCP_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
}