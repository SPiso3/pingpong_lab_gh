#include "utils.h"
#include "../drivers/include/OLED.h"

int main(void) {
	
	UART_init(MYUBRR);
	SPI_master_init();
	uint8_t val;
	//MCP_reset();
	//_delay_ms(2000);
	MCP_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_CONFIG);
	while(1){
		val = MCP_read(MCP_CANSTAT);
		printf("%x\n\r", val);
	}
}


/*
	UART_init(MYUBRR);
	MCP_init();
	MCP_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK); // set loopback mode
	printf("mode: %x\r\n", MCP_read(MCP_CANSTAT));
	MCP_write(0x31, 0xA7);
	MCP_request_to_send(0);
	uint8_t byte = MCP_read(MCP_RXB0SIDH);
	printf("received: %x\r\n", byte);
	*/