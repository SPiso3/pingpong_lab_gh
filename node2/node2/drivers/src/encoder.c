#include "../include/encoder.h"
#include "../include/motor.h"

void encoder_init(){
	PMC->PMC_PCER1 |= (1<<(ID_TC6-32));
	
	PIOC->PIO_PDR |= PIO_PDR_P25;
	PIOC->PIO_PDR |= PIO_PDR_P26;
	
	PIOC->PIO_OER |= PIO_OER_P25;
	PIOC->PIO_OER |= PIO_OER_P26;
	
	PIOC->PIO_ABSR |= PIO_ABSR_P25; //
	PIOC->PIO_ABSR |= PIO_ABSR_P26;
	
	TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN | TC_BMR_EDGPHA;
	TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0; 
	TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

uint32_t encoder_read(){
	return TC2->TC_CHANNEL[0].TC_CV;
}

int32_t get_y(){
	return (-encoder_read() + ENCODER_RANGE/2);
}

int32_t get_r(int8_t pos){
	return (pos * ENCODER_RANGE/2) / JOY_SCALE;
}

