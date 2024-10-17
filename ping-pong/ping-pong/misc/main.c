#include "utils.h"

int main(void) {
	UART_init(MYUBRR);
	MCP_init(MODE_LOOPBACK); // set loopback mode
	
	//create data
	message_t msg = {1, 2, {0xC1, 0xA0, 0x33}};
	
	//send
	CAN_send(&msg);
	
	//check flag
	uint8_t flag = (MCP_read(MCP_CANINTF) & MCP_RX0IF);
	printf("interrupt flag: %x\r\n", flag);
	
	//read data
	message_t rec = CAN_receive();
	printf("received: %x: long %d bytes:: ", rec.id, rec.length);
	
	for(int i=0; i<rec.length; i++){
		printf("%x",rec.data[i]);
	}
	
	//clear flag
	MCP_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
	flag = (MCP_read(MCP_CANINTF) & MCP_RX0IF);
	printf("\n\ris it CLEARED?: int flag: %x\r\n", flag);
}