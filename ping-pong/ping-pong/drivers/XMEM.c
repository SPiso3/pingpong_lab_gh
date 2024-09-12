#include <avr/io.h>
#include "XMEM.h"

void XMEM_init(){
	MCUCR |= (1<<SRE);
	SFIOR |= (1<<XMM2); //XMM=100 - mask PA7-4 for jtag
}

void XMEM_write(uint8_t data, uint16_t addr){
	volatile char *ext_mem = (char *) SRAM_BASE_ADDR;
	ext_mem[addr] = data;
}

uint8_t XMEM_read(uint16_t addr){
	volatile char *ext_mem = (char *) SRAM_BASE_ADDR;
	uint8_t ret_val = ext_mem[addr];
	return ret_val;
}