#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

void SPI_master_init();
void SPI_send(uint8_t cData);
uint8_t SPI_receive();
void SPI_SS_high();
void SPI_SS_low();




#endif /* SPI_H_ */