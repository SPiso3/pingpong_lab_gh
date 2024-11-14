#include "../include/PWM.h"

void pwm_init(void){
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;   // Enable peripheral clock on PWM Controller 
	PIOB->PIO_OER |= PIO_PB13;
	PIOB->PIO_PDR |= PIO_PB13;
	PIOB->PIO_ABSR |= PIO_ABSR_P13;		// Set peripheral MUX to channel B (PWM)

	PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_1024 | PWM_CMR_CPOL; //1024 prescaler + duty==HIGH part
	PWM->PWM_CH_NUM[1].PWM_CPRD = CPRD_VALUE; //20ms
	PWM->PWM_CH_NUM[1].PWM_CDTY = CENTER_PWM;

	PWM->PWM_ENA = PWM_ENA_CHID1; // Enable PWM on Channel 1 (PWMH1 on PB13)
}

void set_duty_cycle(uint8_t value){
	if (value <= MIN_PWM)
		value = MIN_PWM;
	else if (value >= MAX_PWM)
		value = MAX_PWM;
		
	PWM->PWM_CH_NUM[1].PWM_CDTY = value;
}

uint8_t from_joy2pwm(int8_t raw_x){
	//maps from -127+127 to 0.9ms-2.1ms pwm equivalent value
	uint16_t pwm = MIN_PWM + (raw_x + POS_SCALE) * ((MAX_PWM-MIN_PWM)/2) / POS_SCALE;
	
	return (uint8_t)pwm;
}