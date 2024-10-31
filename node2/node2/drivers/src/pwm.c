/*
 *pwm.c
 *
 * Created: 20.10.2021 12:33:04
 *  Author: haakenl
 */ 

#include "sam3x8e.h"
#include "../include/pwm.h"

/* Set up PWM used by the servo */
void pwm_init(void){
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;	// Enable peripheral clock on PWM Controller 
	PIOC->PIO_OER |= PIO_PC19;
	PIOC->PIO_PDR |= PIO_PC19;
	PIOC->PIO_ABSR |= PIO_ABSR_P19;		// Set peripheral MUX to channel B (PWM)
	
	// Setup PWM servo signal
	PWM->PWM_WPCR &= ~PIO_WPMR_WPEN;							// Disable PWM write protection
	REG_PWM_CMR5 = PWM_CMR_CPRE_MCK_DIV_1024 | PWM_CMR_CPOL;	// Set 1024 prescaler and Output wave form start high
	REG_PWM_CPRD5 = CPRD_VALUE;									// CPRD = 20ms x 84MHz/1024
	REG_PWM_CDTY5 = CENTER_PWM;									// CPRY = 1.5ms x 84MHz/1024 (center value between 74 and 172)
	REG_PWM_ENA = PWM_ENA_CHID5;								// Enable PWM
}

/* Up date PWM servo position based on 8-bit unsigned value */
void set_duty_cycle(uint8_t value){
	if (value <= MIN_PWM)
		value = MIN_PWM;
	else if (value >= MAX_PWM)
		value = MAX_PWM;
		
	REG_PWM_CDTY5 = value;
}

uint8_t from_joy2pwm(int8_t raw_x){
	//maps from -127+127 to 0.9ms-2.1ms pwm equivalent value
	uint8_t pwm = MIN_PWM + (raw_x + POS_SCALE) * CENTER_PWM / POS_SCALE;
	return pwm;
}
