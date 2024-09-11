
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

#include "UART.h"

volatile uint8_t received_data;

// Initialize UART
void UART_init(uint16_t ubrr) {

    cli(); //disable interrupts

    UBRR0H = (uint8_t)(ubrr>>8);            //set baud rate
    UBRR0L = (uint8_t)ubrr;
    
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);       //enable receiver and transmitter,
    UCSR0B |= (1<<TXCIE0) | (1<<RXCIE0);    //enable also TXC,RXC interrupts
    
    //frame format: 8 data bits, 2 stop bit, no parity bits
    UUCSRC = (1 << URSEL) | (1<<USBS0) | (3<<UCSZ00);
    sei();  //re-enable global interrupt
}


void UART_transmit(uint8_t data)
{
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

//polling-receive
uint8_t UART_receive()
{
	while (!(UCSR0A & (1 << RXC0)));    //wait until RXC->1

    if ( UCSR0A & (1<<FE)|(1<<DOR)|(1<<UPE) )
        return -1;
    
    PORTB ^= (1<<PB1);                  //blink led
	return UDR0;
}

// Interrupt service routine for receiving data
uint8_t (USART0_RX_vect) {
    return(UDR0);
}

void UART_send_string(const char* str) {
    //iterate until terminator charachter is met
    while (*str) {
        UART_transmit(*str++);
    }
}

ISR(USART_RXC_vect)
{
	PORTB=UDR;
}