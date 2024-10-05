#include "misc/utils.h"
#include "drivers/include/OLED.h"

int main(void) {
	UART_init(MYUBRR);
	XMEM_init();
	OLED_init();
	OLED_reset();
	OLED_clear();
}
