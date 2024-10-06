#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include <avr/io.h>
#include "../include/ADC.h"

#define CALIB_SAMPLES 8
#define IDLE_VAL 50
#define JOY_BUTTON PD5
#define POS_SCALE 127

typedef struct{
	int8_t x;
	int8_t y;
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

void JOY_init();
pos_t JOY_calibrate();
pos_t JOY_get_rel_pos(pos_t center);
dir JOY_get_dir(pos_t rel_pos);
sliders_t JOY_get_sliders();

#endif /* GAMEPAD_H_ */