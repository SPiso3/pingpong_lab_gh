#include "utils.h"

void auto_calib_encoder(){
	printf("CALIBRATING...\n\r");
	drive_motor(+CPRD_VALUE_MOTOR); //right
	time_spinFor(msecs(1000));
	encoder_init();
	ENCODER_MIN = encoder_read();
	
	drive_motor(-CPRD_VALUE_MOTOR); //left
	time_spinFor(msecs(1000));
	ENCODER_MAX = encoder_read();
	
	ENCODER_RANGE = ENCODER_MAX - ENCODER_MIN;
	printf("\rmin: %d MAX: %d range: %d \n\r", ENCODER_MIN, ENCODER_MAX, ENCODER_RANGE);
}