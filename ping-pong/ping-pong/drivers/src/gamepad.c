#include "../include/gamepad.h"
#include "../../misc/utils.h"
extern pos_t gamepad_center;

void JOY_init(){
	DDRD &= ~(1<<JOY_BUTTON); //busy input active low
	PORTD |= (1<<JOY_BUTTON); //pull-up resistor
	JOY_calibrate();
	}

void JOY_calibrate(){
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
	gamepad_center.x = x;
	gamepad_center.y = y;
}

pos_t JOY_get_rel_pos(){
	ADC_sample4();
	int16_t y = ADC_read() - gamepad_center.y;
	int16_t x = ADC_read() - gamepad_center.x;

	y = y * POS_SCALE / ((y > 0) ? (255-gamepad_center.y) : (0+gamepad_center.y));
	x = x * POS_SCALE / ((x > 0) ? (255-gamepad_center.x) : (0+gamepad_center.x));
	
	
	y = MAX(-POS_SCALE,MIN(y,POS_SCALE));
	x = MAX(-POS_SCALE,MIN(x,POS_SCALE));
	
	return (pos_t){x, y};
}

dir JOY_get_dir(){
	pos_t rel_pos = JOY_get_rel_pos();
	
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