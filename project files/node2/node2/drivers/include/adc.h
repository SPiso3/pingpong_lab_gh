#ifndef ADC_H_
#define ADC_H_

#include "sam3x8e.h"
#include "../include/CAN.h"
#include <stdio.h>

#define ADC_THRESHOLD 1500 //TODO tune

void adc_init();

#endif /* ADC_H_ */