#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/ADC.h"

#define CALIB_SAMPLES 8
#define JOY_BUTTON PD5
#define POS_SCALE 127
#define IDLE_VAL (POS_SCALE/2)

#define MAX(a,b) ((a >= b) ? a : b)
#define MIN(a,b) ((a <= b) ? a : b)

typedef struct{
	int16_t x;
	int16_t y;
} pos_t;
	
typedef struct{
	uint8_t left;
	uint8_t right;
} sliders_t;
	
typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	IDLE
	} dir;
	
pos_t gamepad_center;

void JOY_init();
void JOY_calibrate();
pos_t JOY_get_rel_pos();
dir JOY_get_dir();
sliders_t JOY_get_sliders();

#endif /* GAMEPAD_H_ */