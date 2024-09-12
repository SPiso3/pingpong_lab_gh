#ifndef UART_H
#define UART_H

#define F_CPU 4915200UL //4.9152 MHz
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define TXLED PB0
#define RXLED PB1

void UART_init(unsigned int ubrr);

void UART_transmit(char data);
int UART_putchar(char c, FILE *stream);

char UART_receive(void);

#endif // UART_H