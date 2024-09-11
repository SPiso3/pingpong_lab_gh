#include "utils.h"
#include <util\delay.h>

void test_UART()
{
	UART_init(MYUBRR);

	uint8_t ch = 0;
	UART_send_string("Prova trasmissione...\n");
	while(1){
		UART_transmit(ch++);
		_delay_ms(500);
	}
}