#include "misc/utils.h"
#include <avr/io.h>
#include <avr/interrupt.h>

int main(void) {
	test_LB_CAN_main();
}

ISR(INT0_vect) {
	test_LB_CAN_isr();
}