#include "gamepad.h"

pos_t JOY_calibrate(){
	//should have already initialized XMEM, UART, ADC
	printf("\n\rGamePad calibration v 0.1\n\r");
	printf("CENTER : ");
	uint16_t x = 0;
	uint16_t y = 0;
	for(uint8_t i=0; i<CALIB_SAMPLES; i++){
		ADC_sample4();
		y += ADC_read(); //in order: vert-horiz- ...
		x += ADC_read();
	}
	y /= CALIB_SAMPLES;
	x /= CALIB_SAMPLES;
	printf("[%d,%d]\n\r",x,y);
	return (pos_t){x,y};
}

pos_t JOY_get_rel_pos(pos_t center){
	ADC_sample4();
	int16_t y = ADC_read() - center.y;
	if (y>=0){	y = y * 100/(255-center.y);	}
	else{		y = y * 100/(0+center.y);		}
	int16_t x = ADC_read() - center.x;
	if (x>=0){	x = x * 100/(255-center.x);	}
	else{		x = x * 100/(0+center.x);		}
	return (pos_t){x,y};
}

dir JOY_get_dir(pos_t rel_pos){
	if ((abs(rel_pos.x) <= IDLE_VAL) && (abs(rel_pos.y) <= IDLE_VAL)){
		return IDLE;
	}
	else if (abs(rel_pos.y) > abs(rel_pos.x)){
		if (rel_pos.y > 0){ return UP; }
		else {				return DOWN; }
	}
	else{
		if (rel_pos.x > 0){ return RIGHT; }
		else {				return LEFT; }
	}
}

pos_t JOY_get_sliders(){
	ADC_sample4();
	ADC_read(); ADC_read(); //trash the first two
	
	int16_t left = ADC_read();
	int16_t right = ADC_read();
	return (pos_t){left,right};
}