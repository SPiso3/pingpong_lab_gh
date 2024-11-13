#ifndef PWM_H_
#define PWM_H_

#include "sam3x8e.h"
#include "UART.h"

#define PWM_PERIOD 20		//ms
#define CPRD_VALUE 1640+1		//CPRD = 20ms x 84MHz/1024

#define MIN_PWM 74			//0.9*X/CPRD
#define MAX_PWM 172
#define CENTER_PWM (MIN_PWM+(MAX_PWM-MIN_PWM)/2)-2

#define POS_SCALE 127

void pwm_init(void);
void set_duty_cycle(uint8_t value);
uint8_t from_joy2pwm(int8_t raw_x);

#endif /* PWM_H_ */