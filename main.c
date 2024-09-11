/*
 * PINGPONG_project.c
 *
 * Created: 05.09.2024 09:26:10
 * Authors : Adriaes ..., Alberto Morselli, Sergio Enrico Pisoni
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "UART.h"

int main(void)
{
	UART_init(MYUBRR);
	DDRB |= (1 << PB1);
	PORTB &= ~(1 << PB0);

	uint8_t ch = 0;
	UART_send_string("Prova trasmissione...\n");
    for(int i=0; i<26; i++){
		UART_transmit(ch++);
        PORTB ^= (1 << PB0);
		_delay_ms(500);
    }

    UART_send_string("\n\nProva ricezione...\n");
    while(1){
        
    }
    return 0;
}