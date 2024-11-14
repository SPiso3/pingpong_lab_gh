#ifndef MOTOR_H_
#define MOTOR_H_

#include "sam3x8e.h"

#define CPRD_VALUE_MOTOR 3360 //25khz - 40usec

void motor_init();
void drive_motor(uint16_t u);

#endif /* MOTOR_H_ */