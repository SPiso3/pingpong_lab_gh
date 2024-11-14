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
#include "drivers/include/solenoid.h"
#include "drivers/include/motor.h"
#include "utils.h"

int16_t ENCODER_MIN, ENCODER_MAX, ENCODER_RANGE;
joy_pos_t joy_pos_rec;
controller_t controller;
bool gameloop = false;
bool do_calib = false;

#define FPS 60
#define SAMPLE_TIME 1000/FPS

void run(){
	time_spinFor(msecs(SAMPLE_TIME));
	
	uint8_t pwm_value = from_joy2pwm(-*controller.servo);	//flipped direction (-)
	set_duty_cycle(pwm_value);								//control servo
	
	int32_t y = get_y();									//measure from encoder
	int32_t r = get_r(*controller.motor);					//rescale reference
	int32_t u = pid(r,y);
	drive_motor(u);											//drive motor

	//handle solenoid
	if(!joy_pos_rec.btn){
		solenoid_push();
	}
	else{
		solenoid_release();
	}
}

int main(void) {
	SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
	uart_init(F_CPU, 9600);
	can_init((CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 1, .sjw=2}, 1);
	adc_init();
	
	pwm_init();
	motor_init();
	solenoid_init();
	
	//default if not sent
	controller.motor = &joy_pos_rec.sr;
	controller.servo = &joy_pos_rec.x;
	
	//ZzZ...

	while (1){
		if(gameloop){
			if (do_calib){
				do_calib = false;
				auto_calib_encoder();
			}
			run();
		}
		else {
			printf("zZz...\n\r");
			time_spinFor(msecs(1000));
		}
	}
}