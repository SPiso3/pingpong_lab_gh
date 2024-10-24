#include "../include/CAN.h"


void CAN_init(){
	MCP_init(MODE_LOOPBACK);
	
	//sei();
}

void CAN_send(message_ptr msg){
	//set ID 
	MCP_write(MCP_TXB0SIDH, msg->id >> 3);
	MCP_write(MCP_TXB0SIDL, (msg->id % 8) << 5);
	
	//set length
	MCP_write(MCP_TXB0DLC, msg->length);
	
	//store data
	for(uint8_t i = 0; i < msg->length; i++){
		MCP_write(MCP_TXB0D0 + i,msg->unsigned_data[i]);
	}
	
	//request to send
	MCP_request_to_send(0);
}

message_t CAN_receive(){
	message_t msg = {};
		
	//get ID
	uint8_t id_low = (MCP_read(MCP_RXB0SIDL)>>5);
	uint8_t id_high = MCP_read(MCP_RXB0SIDH);
	msg.id = (id_high << 3) | id_low;
	
	//get data length 
	msg.length = MCP_read(MCP_RXB0DLC);
	
	//get data
	for (int i = 0; i < msg.length; i++) {
		msg.unsigned_data[i] = MCP_read(MCP_RXB0D0 + i);
	}
	
	return msg;
}

