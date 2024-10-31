#include "sam3x8e.h"

#include <stdio.h>
#include "drivers/include/uart.h"
#include "drivers/include/can.h"
#include "drivers/include/time.h"
#include "drivers/include/pwm.h"

int main(void) {
	SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
	uart_init(F_CPU, 9600);
	can_init((CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 1, .sjw=2}, 0);
	time_spinFor(msecs(10));
	
	pwm_init();
	set_duty_cycle(100);

	CanMsg can_msg;
	while (1){
		
		if(can_rx(&can_msg)){
			can_print_JOY(can_msg);
			int8_t joy_x = can_msg.signed_data[0];
			printf("%d\n\r",joy_x);
			uint8_t pwm_value = from_joy2pwm(-joy_x);
			printf("%d\n\r",pwm_value);
			set_duty_cycle(pwm_value);
		}
		
		time_spinFor(msecs(50));
	}
}