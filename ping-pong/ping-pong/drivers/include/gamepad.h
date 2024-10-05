#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "ADC.h"

#define CALIB_SAMPLES 8
#define IDLE_VAL 50
#define JOY_BUTTON PD5

typedef struct{
	int16_t x;
	int16_t y;
	} pos_t;
	
typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	IDLE
	} dir;

void JOY_init();
pos_t JOY_calibrate();
pos_t JOY_get_rel_pos(pos_t center);
dir JOY_get_dir(pos_t rel_pos);
pos_t JOY_get_sliders();

#endif /* GAMEPAD_H_ */