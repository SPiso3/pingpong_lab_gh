#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "drivers/UART.h"
#include "drivers/XMEM.h"
#include <util/delay.h>

void test_UART(){
	UART_init(MYUBRR);
	
	printf("\nProva trasmissione\n\r");
	int n = 0;
	while(1){
		printf("%d\n\r",n++);
		_delay_ms(200);
	}
}

void test_XMEM(){
	XMEM_init();
	UART_init(MYUBRR);
	uint16_t addr = 0x1000;
	while (1){
		printf("address: %x\n\r",addr);
		XMEM_write(0xFF, addr++);
		_delay_ms(2);
	}
}