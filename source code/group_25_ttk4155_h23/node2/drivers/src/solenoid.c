#include "../include/solenoid.h"

void solenoid_init() {
	PIOA->PIO_PER = PIO_PER_P3;
	PIOA->PIO_OER = PIO_OER_P3;
	
	PIOA->PIO_SODR = PIO_SODR_P3; //start by setting to 1 = OFF (ative low)
}

void solenoid_push() {
	PIOA->PIO_CODR = PIO_CODR_P3;
}

void solenoid_release() {
	PIOA->PIO_SODR = PIO_SODR_P3;
}