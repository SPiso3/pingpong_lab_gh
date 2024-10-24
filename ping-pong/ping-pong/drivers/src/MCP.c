#include "../include/MCP.h"

void MCP_init(uint8_t mode){
	SPI_master_init();
	MCP_reset();
	
	// Self-test
	uint8_t val = MCP_read(MCP_CANSTAT);
	if((val & MODE_MASK) != MODE_CONFIG) {
		printf("\nMCP2515 is NOT in configuration mode after reset!\n\r");
		printf("read value: %x should be: %x\n\r", val, MODE_CONFIG);
		return;
	}
	
	printf("\nMCP2515 is in configuration mode...\n\r");
	MCP_write(MCP_CNF1, 63); //125kbps
	MCP_bit_modify(MCP_CANINTE, 0xFF, MCP_RX0IF); //enable ONLY rx interrupt on buffer 0
	
	MCP_bit_modify(MCP_CANCTRL, MODE_MASK, mode);
	printf("set to MODE: %x\n\r",mode);
}

void MCP_reset(){
	SPI_SS_low();
	SPI_send(MCP_RESET);
	SPI_SS_high();
}

uint8_t MCP_read(uint8_t address) {
	SPI_SS_low();
	
	SPI_send(MCP_READ); //single-register read
	SPI_send(address);
	uint8_t data = SPI_receive();
	
	SPI_SS_high();
	
	return data;
}

void MCP_write(uint8_t address, uint8_t data) {
	SPI_SS_low();
	
	SPI_send(MCP_WRITE); //single-register write
	SPI_send(address);
	SPI_send(data);
	
	SPI_SS_high();
}

void MCP_request_to_send(uint8_t buffer_number) { // send function called by CAN driver 
	SPI_SS_low();
	buffer_number = buffer_number % 3; // Map buffer number (for loops)
	char data = MCP_RTS_TX0;
	
	if (buffer_number == 0) {
		data = MCP_RTS_TX0;
	}
	else if (buffer_number == 1) {
		data = MCP_RTS_TX1;
	}
	else if (buffer_number == 2) {
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