#include "sam3x8e.h"

#include <stdio.h>
#include <stdlib.h>
#include "drivers/include/UART.h"
#include "drivers/include/CAN.h"
#include "drivers/include/time.h"
#include "drivers/include/PWM.h"
#include "drivers/include/ADC.h"
#include "drivers/include/encoder.h"
#include "drivers/include/pid.h"

extern joy_pos_t joy_pos_rec;

int main(void) {
	SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
	uart_init(F_CPU, 9600);
	can_init((CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 1, .sjw=2}, 1);
	pwm_init();
	adc_init();
	set_duty_cycle(CENTER_PWM);
	encoder_init();
	solenoid_init();
	motor_init();
	

	while (1){
		time_spinFor(msecs(16));
		
		uint8_t pwm_value = from_joy2pwm(-joy_pos_rec.x);
		set_duty_cycle(pwm_value);
		
		int32_t y = (-encoder_read() + 2813);
		//printf("%d\n\r",rec_pos_sl);
		int32_t r = (joy_pos_rec.y)*2813/127;
		int32_t u = pid(r,y);
		int8_t dir = (u >= 0) ? 1 : -1;
		u = (abs(u) >= 3360) ? 3360 : abs(u);
		//CanMsg can_msg = (CanMsg){.id=CAN_ID_PWM, .length=1, .unsigned_data={u}};
		//can_tx(can_msg);
		
		drive_motor(u);
		if (dir >= 0)
			PIOC->PIO_SODR |= PIO_PER_P23;
		else
			PIOC->PIO_CODR |= PIO_PER_P23;
			
		if(!joy_pos_rec.btn){
			PIOA->PIO_CODR = PIO_CODR_P3;
		}
		else{
			PIOA->PIO_SODR = PIO_SODR_P3;
		}
			
		//uint16_t data = ADC -> ADC_CDR[0];
		//printf("%d\n\r", data);
		
		//printf("%d %d S: %d %d BTN: %d\n\r", joy_pos_rec.x, joy_pos_rec.y, joy_pos_rec.sl, joy_pos_rec.sr, joy_pos_rec.btn);
	}
}

//max 5627