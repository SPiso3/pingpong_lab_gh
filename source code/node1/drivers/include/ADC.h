#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <avr/io.h>
#include <stdbool.h>

#define ADC_BASE_ADDR 0x1400
#define ADC_CLOCK PB0
#define ADC_BUSY PD4
#define ADC_CHANNELS 4

bool ADC_is_busy();
void ADC_init();
void ADC_sample4();

uint8_t ADC_read(void);
uint8_t ADC_read_channel(uint8_t channel);

#endif /* ADC_H_ */