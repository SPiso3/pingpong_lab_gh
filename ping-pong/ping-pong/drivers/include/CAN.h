#ifndef CAN_H_
#define CAN_H_

#include "../include/MCP.h"


typedef struct Message {
	unsigned int id; 
	uint8_t length;  //of data
	char data[8];    
} message_t, *message_ptr;

#endif /* CAN_H_ */