#ifndef XMEM_H_
#define XMEM_H_

#define SRAM_BASE_ADDR 0x1800

void XMEM_init();
void XMEM_write(uint8_t data, uint16_t addr);
uint8_t XMEM_read(uint16_t addr);

#endif /* XMEM_H_ */