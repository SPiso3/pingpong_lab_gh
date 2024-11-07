#include "sam3x8e.h"

#include <stdio.h>
#include "drivers/include/uart.h"
#include "drivers/include/can.h"
#include "drivers/include/time.h"
#include "drivers/include/pwm.h"
#include "drivers/include/adc.h"

int main(void) {
	SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
	uart_init(F_CPU, 9600);
	can_init((CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 1, .sjw=2}, 0);
	pwm_init();
	adc_init();

	CanMsg can_msg;
	while (1){
		
		if(can_rx(&can_msg)){
			can_print_JOY(can_msg);
			int8_t joy_x = can_msg.signed_data[0];
			uint8_t pwm_value = from_joy2pwm(-joy_x);
			//printf("\n\rpwm: %d\n\r",pwm_value);
			set_duty_cycle(pwm_value);
			can_msg = (CanMsg){.id=0xDD, .length=1, .dword={pwm_value}};
			can_tx(can_msg);
		}
		//uint32_t data = ADC -> ADC_CDR[0];
		//printf(" ADC: %d\n\r",data);
		time_spinFor(msecs(100));
	}
}