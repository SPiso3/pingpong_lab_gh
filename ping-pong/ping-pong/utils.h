#ifndef UTILS_H_
#define UTILS_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "drivers/UART.h"
#include "drivers/XMEM.h"
#include "drivers/ADC.h"
#include <util/delay.h>

void test_UART();
void test_SRAM();
void test_XMEM();
void test_ADC();

#endif /* UTILS_H_ */