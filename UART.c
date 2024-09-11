
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "UART.h"

volatile unsigned char received_data;

// Initialize UART
void UART_init(uint16_t ubrr) {
    //set baud rate
    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;
    
    //enable receiver and transmitter, enable RX interrupt
    UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
    
    //frame format: 8 data bits, 1 stop bit, no parity bits
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
    sei();  //re-enable global interrupt
}

void UART_init(uint16_t ubrr)
{
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);
}

void UART_transmit(uint8_t data)
{
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

uint8_t UART_receive()
{
	while (!(UCSR0A & (1 << RXC0)));    //wait until busy
	PORTB ^= (1<<PB1);                  //blink led
	return(UDR0);
}

// Interrupt service routine for receiving data
ISR(USART0_RX_vect) {
    received_data = UDR0; // Store received data in a volatile variable
}

void UART_send_string(const char* str) {
    //iterate until terminator charachter is met
    while (*str) {
        UART_transmit(*str++);
    }
}