#include "sam3x8e.h"

#include <stdio.h>
#include "drivers/include/uart.h"
#include "drivers/include/can.h"

#define F_CPU 84000000
void delay(volatile uint32_t count) {
	while (count--) {
		__asm__("nop"); // No operation for delay
	}
}


int main(void) {
	SystemInit();
	uart_init(F_CPU, 9600);
	can_init((CanInit){.brp = F_CPU/2000000-1, .phase1 = 5, .phase2 = 1, .propag = 6}, 0);
	while(1){
		printf("prova\n\r");
		delay(10000000);
	}
	return 0;
}


