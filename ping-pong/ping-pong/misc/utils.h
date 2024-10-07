#ifndef UTILS_H_
#define UTILS_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../drivers/include/UART.h"
#include "../drivers/include/XMEM.h"
#include "../drivers/include/ADC.h"
#include "../drivers/include/gamepad.h"
#include "../drivers/include/OLED.h"
#include "../drivers/include/menu.h"
#include <util/delay.h>

#define MAX(a,b) ((a >= b) ? a : b)
#define MIN(a,b) ((a <= b) ? a : b)

void test_UART();
void test_SRAM();
void test_XMEM();
void test_ADC();
void test_JOYSTICK();
void test_OLED();
void test_MENU();

#endif /* UTILS_H_ */