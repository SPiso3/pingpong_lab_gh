
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "UART.h"

volatile uint8_t data; //to store rx data that can change at anytime (external) use volatile

//--------

void UART_init(uint16_t ubrr) {

	cli(); //disable interrupts

	UBRR0H = (uint8_t)(ubrr>>8);	//set baud rate
	UBRR0L = (uint8_t)ubrr;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);       //enable receiver and transmitter...
	UCSR0B |= (1<<TXCIE0) | (1<<RXCIE0);    //...and also TXC,RXC interrupts
	
	UCSR0C = (1 << URSEL0); //access UCSRC register (shared with UBRHH)
	UCSR0C |= (3<<UCSZ00) | (1<<USBS0) | (1<<UPM11); //frame format: 8 data bits, 2 stop bit, parity: enabled, type = EVEN
	
	sei();  //re-enable global interrupt
	
	//set led pins
	DDRB |= (1<<TXLED) | (1<<RXLED);	//direction: output
	PORTB |= (1<<TXLED) | (1<<RXLED);	//turn them off (sink-mode)
}

//--------

void UART_transmit(uint8_t data){
	while (!(UCSR0A & (1<<UDRE0))); //wait until UDRE->1 = ready
	UDR0 = data;
}

void UART_send_string(const char *str){
	while (*str){
		UART_transmit(*str++);
	}
}

ISR(USART0_TXC_vect) {
	PORTB ^= (1<<TXLED); //blink as response of successful transmission
}

//----------

uint8_t UART_receive(){
	//frame-error OR data-over-run OR parity-error
	if ( UCSR0A & ((1<<FE0)|(1<<DOR0)|(1<<UPE0)) )
		return -1;
	
	PORTB ^= (1<<RXLED); //blink led to report success of rx
	return UDR0;
}

// Interrupt service routine for receiving data
ISR(USART0_RXC_vect) {
	data = UART_receive();
}