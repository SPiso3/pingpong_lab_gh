#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 4915200UL //4.9152 MHz

#include <util/delay.h>
#include "drivers/UART.h"
#include "utils.h"

int main(void)
{
	test_UART();
    return 0;
}