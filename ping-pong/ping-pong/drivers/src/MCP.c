#include "../include/MCP.h"

void MCP_init(){
	SPI_master_init();
	MCP_reset();
	
	
}

uint8_t MCP_read(uint8_t address) { //read from MCP buffer
	SPI_SS_low();
	SPI_send(MCP_READ);
	SPI_send(address);
	uint8_t data = SPI_receive();
	SPI_SS_high();
	
	return data;
}

void MCP_write(uint8_t address, uint8_t data) { // write to a MCP buffer
	SPI_SS_low();
	SPI_send(MCP_WRITE);
	SPI_send(address);
	SPI_send(data);
	SPI_SS_high();
}

void MCP_request_to_send(int buffer_number) { // send function called by CAN driver 
	SPI_SS_low();
	buffer_number = buffer_number % 3; // Map buffer number (for loops)
	char data = MCP_RTS_TX0;
	if (buffer_number == 0) {
		data = MCP_RTS_TX0;
		} else if (buffer_number == 1) {
		data = MCP_RTS_TX1;
		} else if (buffer_number == 2) {
		data = MCP_RTS_TX2;
	}
	SPI_send(data);
	SPI_SS_high();
}

char MCP_read_status() {
	SPI_SS_low();
	SPI_send(MCP_READ_STATUS);
	char data = SPI_receive();
	SPI_SS_high();

	return data;
}

void MCP_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
	SPI_SS_low();
	SPI_send(MCP_BITMOD);
	SPI_send(address);
	SPI_send(mask);
	SPI_send(data);
	SPI_SS_high();
}

void MCP_reset(){
	SPI_SS_low();
	SPI_send(MCP_RESET);
	SPI_SS_high();
}