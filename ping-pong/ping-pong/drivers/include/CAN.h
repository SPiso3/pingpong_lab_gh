#ifndef CAN_H_
#define CAN_H_

#include "../include/MCP.h"
#include <avr/interrupt.h>

typedef enum {
	CAN_ID_GOAL,
	CAN_ID_JOYSTICK,
	CAN_ID_PWM,
} Can_Id;

typedef struct Message {
	uint16_t id; 
	uint8_t length;  //of data
	union{	
		uint8_t unsigned_data[8];
		int8_t signed_data[8];
		char text[8];
	};
	
} message_t, *message_ptr;

void CAN_init();
void CAN_send(message_ptr msg);
message_t CAN_receive();

#endif /* CAN_H_ */