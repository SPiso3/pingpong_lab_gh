#ifndef MOTOR_H_
#define MOTOR_H_

#include "sam3x8e.h"
#include <stdlib.h>

#define CPRD_VALUE_MOTOR 3360 //25khz - 40usec

void motor_init();
void drive_motor(int16_t u);

#endif /* MOTOR_H_ */