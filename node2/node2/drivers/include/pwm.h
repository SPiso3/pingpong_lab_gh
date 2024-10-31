/*
 * pwm.h
 *
 * Created: 20.10.2021 12:32:38
 *  Author: noi
 */ 

#ifndef PWM_H_
#define PWM_H_

#include "uart.h"

#define PWM_PERIOD 20		//ms
#define CPRD_VALUE 1648		//CPRD = 20ms x 84MHz/1024

#define MIN_PWM 74
#define MAX_PWM 172
#define CENTER_PWM (MAX_PWM-MIN_PWM)/2

#define POS_SCALE 127

void pwm_init(void);
void set_duty_cycle(uint8_t value);
uint8_t from_joy2pwm(int8_t raw_x);

#endif /* PWM_H_ */