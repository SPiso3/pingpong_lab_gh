#ifndef XMEM_H_
#define XMEM_H_

#include <avr/io.h>

#define XMEM_BASE_ADDR 0x1000
#define SRAM_BASE_ADDR 0x1800

void XMEM_init();
void XMEM_write(uint8_t data, uint16_t addr);
uint8_t XMEM_read(uint16_t addr);

#endif /* XMEM_H_ */