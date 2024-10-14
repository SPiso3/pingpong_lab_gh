#include "../include/SPI.h"

#define DDR_SPI DDRB
#define SS PB4
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7

void SPI_master_init(){
	// Set MOSI, SCK and !SS output, all others input 
	DDR_SPI |= (1<<DD_MOSI)|(1<<DD_SCK)|(1<<SS);
	// Enable SPI, Master, set clock rate fck/16, TODO enable interrupts
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	// explicitly setting mode 00
	SPCR &= ~((1 << CPOL) | (1 << CPHA));
	// !SS to 1 
	SPI_SS_high(); 
}

void SPI_send(uint8_t cData){  //remember to set SS low before calling
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF))){}
}

uint8_t SPI_receive() {
	//send dummy
	SPDR = 0xAA;
	while(!(SPSR & (1<<SPIF))){}
	
	// Return data register
	return SPDR;
}

void SPI_SS_high(){
	PORTB |= (1 << SS);
}

void SPI_SS_low(){
	PORTB &= ~(1 << SS);
}