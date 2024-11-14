#include "../include/motor.h"

void motor_init(){
	//settings for enable/pwm
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;				// enable peripheral clock on PWM controller 
	
	PIOB->PIO_OER |= PIO_PB12;
	PIOB->PIO_PDR |= PIO_PB12;
	PIOB->PIO_ABSR |= PIO_ABSR_P12;					// set peripheral MUX to channel B (PWM)

	PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPOL;		// no prescaler, HIGH=duty
	PWM->PWM_CH_NUM[0].PWM_CPRD = CPRD_VALUE_MOTOR; //set period
	PWM->PWM_CH_NUM[0].PWM_CDTY = 0;

	PWM->PWM_ENA |= PWM_ENA_CHID0;					//enable
	
	//settings for phase/dir
	PMC->PMC_PCER0 |= PMC_PCER0_PID13;				//clock power management
	PIOC->PIO_OER |= PIO_OER_P23;
	PIOC->PIO_PER |= PIO_PER_P23;
}

void drive_motor(uint16_t u){
	PWM->PWM_CH_NUM[0].PWM_CDTY = u;
}