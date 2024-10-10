#ifndef MCP_H_
#define MCP_H_

#include "../include/SPI.h"
#include "../include/MCP2515.h"


void MCP_init();
uint8_t MCP_read(uint8_t address);
void MCP_write(uint8_t address, uint8_t data);
void MCP_request_to_send(int buffer_number);
char MCP_read_status();
void MCP_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
void MCP_reset();


#endif /* MCP_H_ */