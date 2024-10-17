#ifndef CAN_H_
#define CAN_H_

#include "../include/MCP.h"
#include <avr/interrupt.h>

typedef struct Message {
	uint16_t id; 
	uint8_t length;  //of data
	uint8_t data[8];    
} message_t, *message_ptr;

void CAN_init();
void CAN_send(message_ptr msg);
message_t CAN_receive();

#endif /* CAN_H_ */