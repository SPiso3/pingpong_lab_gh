#include "sam3x8e.h"

#include <stdio.h>
#include "drivers/include/UART.h"
#include "drivers/include/CAN.h"
#include "drivers/include/time.h"
#include "drivers/include/PWM.h"
#include "drivers/include/ADC.h"
#include "drivers/include/encoder.h"
#include "drivers/include/pid.h"

extern int8_t rec_pos_x;
extern int8_t rec_pos_y;
extern uint8_t rec_pos_sl;

int main(void) {
	SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
	uart_init(F_CPU, 9600);
	can_init((CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 1, .sjw=2}, 1);
	pwm_init();
	adc_init();
	set_duty_cycle(CENTER_PWM);
	encoder_init();
	
	PMC->PMC_PCER0 |= PMC_PCER0_PID13;
	PIOC->PIO_OER |= PIO_OER_P23;
	PIOC->PIO_PER |= PIO_PER_P23;

	while (1){
		time_spinFor(msecs(16));
		
		uint8_t pwm_value = from_joy2pwm(-rec_pos_y);
		set_duty_cycle(pwm_value);
		
		int32_t y = -encoder_read() + 2813;
		//printf("%d\n\r",rec_pos_sl);
		int32_t r = (rec_pos_x)*2813/127;
		int32_t u = pid(r,y);
		int8_t dir = (u >= 0) ? 1 : -1;
		u = (abs(u) >= 3360) ? 3360 : abs(u);
		
		PWM->PWM_CH_NUM[0].PWM_CDTY = u;
		if (dir >= 0)
			PIOC->PIO_SODR |= PIO_PER_P23;
		else
			PIOC->PIO_CODR |= PIO_PER_P23;
			
		//uint32_t data = ADC -> ADC_CDR[0];
		//printf(" ADC: %d\n\r", (uint16_t)data;
	}
}

//max 5627