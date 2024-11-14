#ifndef ENCODER_H_
#define ENCODER_H_

#include "sam3x8e.h"
#include <stdio.h>

#define JOY_SCALE 127;
extern int16_t ENCODER_MIN, ENCODER_MAX, ENCODER_RANGE;

void encoder_init();
uint32_t encoder_read();

int32_t get_y();
int32_t get_r(int8_t pos);

#endif /* ENCODER_H_ */