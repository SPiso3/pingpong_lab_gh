#include "../include/gamepad.h"
#include "../../misc/utils.h"

void JOY_init(){
	DDRD &= ~(1<<JOY_BUTTON); //busy input active low
	PORTD |= (1<<JOY_BUTTON); //pull-up resistor
	}

pos_t JOY_calibrate(){
	//should have already initialized XMEM, UART, ADC
	printf("\n\rcalibrating gamepad... ");
	uint16_t x = 0;
	uint16_t y = 0;
	
	for(uint8_t i=0; i<CALIB_SAMPLES; i++){
		ADC_sample4();
		y += ADC_read(); //in order: vert-horiz- ...
		x += ADC_read();
	}
	y /= CALIB_SAMPLES;
	x /= CALIB_SAMPLES;
	
	printf("done. CENTER = [%d,%d]\n\r",x,y);
	return (pos_t){x,y};
}

pos_t JOY_get_rel_pos(pos_t center){
	ADC_sample4();
	int16_t y = ADC_read() - center.y;
	int16_t x = ADC_read() - center.x;

	y *= POS_SCALE / ((x > 0) ? (255-center.x) : (0+center.x));
	x *= POS_SCALE / ((x > 0) ? (255-center.x) : (0+center.x));
	
	y = MAX(-POS_SCALE,MIN(y,POS_SCALE));
	x = MAX(-POS_SCALE,MIN(x,POS_SCALE));
	
	return (pos_t){(int8_t)x, (int8_t)y};
}

dir JOY_get_dir(pos_t rel_pos){
	if ((abs(rel_pos.x) <= IDLE_VAL) && (abs(rel_pos.y) <= IDLE_VAL)){
		return IDLE;
	}
	else if (abs(rel_pos.y) > abs(rel_pos.x)){
		return (rel_pos.y > 0) ? UP : DOWN;
	}
	return (rel_pos.x > 0) ? RIGHT : LEFT;
}

sliders_t JOY_get_sliders(){
	ADC_sample4();
	ADC_read(); ADC_read(); //trash the first two
	
	uint8_t left = ADC_read();
	uint8_t right = ADC_read();
	return (sliders_t){left,right};
}