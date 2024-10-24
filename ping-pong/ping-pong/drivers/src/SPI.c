#include "../include/SPI.h"

void SPI_master_init(){
	// Set MOSI, SCK and !SS output
	DDR_SPI |= (1<<DD_MOSI)|(1<<DD_SCK)|(1<<SS);
	
	// Enable SPI, Master, set clock rate fck/16, TODO? enable interrupts
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);//|(1<<SPIE);
	SPCR &= ~((1 << CPOL) | (1 << CPHA)); // explicitly setting mode 00
	SPI_SS_high(); // !SS to 1 
}

void SPI_send(uint8_t data){ 
	//remember to set SS low before calling
	SPDR = data;
	while(!(SPSR & (1<<SPIF))){} //Wait for transmission complete
}

uint8_t SPI_receive() {
	SPI_send(0xAA);		//send dummy
	return SPDR;		// Return data register
}

void SPI_SS_high(){
	PORT_SPI |= (1 << SS);
}

void SPI_SS_low(){
	PORT_SPI &= ~(1 << SS);
}