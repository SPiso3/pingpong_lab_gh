#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdio.h>

#define DDR_SPI DDRB
#define PORT_SPI PORTB

#define SS PB4
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7

void SPI_master_init();
void SPI_send(uint8_t data);
uint8_t SPI_receive();
void SPI_SS_high();
void SPI_SS_low();

#endif /* SPI_H_ */