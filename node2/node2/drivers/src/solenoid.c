#include "../include/solenoid.h"


void solenoid_init() {
	PIOA->PIO_PER = PIO_PER_P3;
	PIOA->PIO_OER = PIO_OER_P3;
	PIOA->PIO_SODR = PIO_SODR_P3;
}

void solenoid_hit() {
	
}