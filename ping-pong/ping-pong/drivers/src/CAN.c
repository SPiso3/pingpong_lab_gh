#include "../include/CAN.h"


void CAN_init(){
	MCP_init();
	MCP_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK); // set loopback mode
	
	MCP_bit_modify(MCP_CANINTE, 0b11111111, MCP_RX0IF); // enable rx!
	
	sei();
}

// TODO, can send message, receive 


