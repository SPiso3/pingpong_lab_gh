#include "UART.h"

volatile char received_data; //to store rx data that can change at anytime (external) use volatile

//--------

void UART_init(unsigned int ubrr) {

	cli(); //disable interrupts

	UBRR0H = (unsigned char)(ubrr>>8);	//set baud rate
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<TXCIE0) | (1<<RXCIE0);  //enable RX,TX and TXC,RXC interrupts
	
	UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3<<UCSZ00); //frame format: 8 data bits, 2 stop bit, parity: none
	
	// redirecting stout
	FILE *uart_output = fdevopen(UART_putchar, NULL);
	stdout = uart_output;
	
	sei();  //re-enable global interrupt
	
	//set led pins
	DDRB |= (1<<TXLED) | (1<<RXLED);	//direction: output
	PORTB |= (1<<TXLED) | (1<<RXLED);	//turn them off (sink-mode)
}

//-------- TRANSMISSION -----------

ISR(USART0_TXC_vect) {
	PORTB ^= (1<<TXLED); //blink as response of successful transmission
}

void UART_transmit(char data){
	while (!(UCSR0A & (1<<UDRE0))); //wait until UDRE->1 = ready
	UDR0 = data;
}

// using two different functions transmit data (maybe unify them under putchar) / maybe adding ring buffer to write data also
int UART_putchar(char c, FILE *stream)
{	UART_transmit(c);
	return 0;
}

//---------- RECEIVING ------------

char UART_receive(){
	//frame-error OR data-over-run OR parity-error
	if ( UCSR0A & ((1<<FE0)|(1<<DOR0)|(1<<UPE0)) )
		return -1;
	
	PORTB ^= (1<<RXLED); //blink led to report success of rx
	return UDR0;
}

// Interrupt service routine for receiving data
ISR(USART0_RXC_vect) {
	received_data = UART_receive();
	printf("@echo: %c\n\r", received_data);
}