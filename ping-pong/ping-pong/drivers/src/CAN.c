#include "../include/CAN.h"


void CAN_init(){
	MCP_init();
	MCP_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK); // set loopback mode

	MCP_bit_modify(MCP_CANINTE, 0b11111111, 0b1); // enable rx!
}

// TODO, can send message, receive 


