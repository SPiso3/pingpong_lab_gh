#include "utils.h"
#include "drivers/gamepad.h"

int main(void)
{	
	UART_init(MYUBRR);
	XMEM_init();
	ADC_init();
	
	pos_t center = JOY_calibrate();
	_delay_ms(1000);
	
	while(1){
		pos_t pos = JOY_get_rel_pos(center);
		dir direction = JOY_get_dir(pos);
		pos_t sliders = JOY_get_sliders();
		
		printf("\x1B[2J\x1b[H");
		printf("x %d\n\r",pos.x);
		printf("y %d\n\r",pos.y);
		
		printf("DIR: %d\n\r",direction);
		
		printf("sliderL %d\n\r",sliders.x);
		printf("sliderR %d\n\r",sliders.y);
		_delay_ms(100);
	}
}