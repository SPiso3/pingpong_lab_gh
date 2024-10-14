#ifndef CAN_H_
#define CAN_H_

#include "../include/MCP.h"
#include <avr/interrupt.h>

typedef struct Message {
	unsigned int id; 
	uint8_t length;  //of data
	char data[8];    
} message_t, *message_ptr;

void CAN_init();

#endif /* CAN_H_ */