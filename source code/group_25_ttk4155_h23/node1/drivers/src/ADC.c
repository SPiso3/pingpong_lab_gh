#include "../include/XMEM.h"
#include "../include/ADC.h"

void ADC_init(){
	DDRB |= (1<<ADC_CLOCK); //clock
	DDRD &= ~(1<<ADC_BUSY); //busy input active low
	PORTD |= (1<<ADC_BUSY); //pull-up resistor
}

void ADC_sample4(){
	volatile char *adc = (char *) ADC_BASE_ADDR;
	adc[0x0000] = 0x00;
}

bool ADC_is_busy(){
	return PIND & (1<<ADC_BUSY);
}

uint8_t ADC_read_channel(uint8_t channel){
	volatile char *adc = (char *) ADC_BASE_ADDR;
	uint8_t ret_val = 0;
	for(uint8_t i=0; (i<ADC_CHANNELS) && (i<=channel); i++){
		ret_val = adc[0x0000]; //discard first channel-1 values
	}
	return ret_val;	//return last one
}

uint8_t ADC_read(void){
	volatile char *adc = (char *) ADC_BASE_ADDR;
	while(!ADC_is_busy()); //wait till busy goes up
	return adc[0x0000];
}