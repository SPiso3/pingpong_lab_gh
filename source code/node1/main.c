#include "drivers/include/menu.h"

int main(void) {
	UART_init(MYUBRR);
	XMEM_init();
	ADC_init();
	JOY_init();
	CAN_init();
	OLED_init();
	OLED_reset();
	
	menu_init();
}