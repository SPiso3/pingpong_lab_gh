#include "../include/CAN.h"
#include "../include/PWM.h"
#include "../../utils.h"

extern joy_pos_t joy_pos_rec;
extern controller_t controller;
extern bool gameloop, do_calib;

void can_printmsg(CanMsg m){
    printf("\n\rCanMsg(id:%d, length:%d, data:{", m.id, m.length);
    if(m.length){
        printf("%d", m.byte[0]);
    }
    for(uint8_t i = 1; i < m.length; i++){
        printf(", %d", m.byte[i]);
    }
    printf("})");
}

void can_print_JOY(CanMsg m){
	printf("\n\rCanMsg(id:%d, length:%d, data:{", m.id, m.length);
		if(m.length){
			for(uint8_t i = 0; i < m.length; i++)
				printf(", %d", m.signed_data[i]);
		}
	printf("})");
}

#define txMailbox 0
#define rxMailbox 1

void can_init(CanInit init, uint8_t rxInterrupt){
    // Disable CAN
    CAN0->CAN_MR &= ~CAN_MR_CANEN; 
    
    // Clear status register by reading it
    __attribute__((unused)) uint32_t ul_status = CAN0->CAN_SR;     
    
    // Disable interrupts on CANH and CANL pins
    PIOA->PIO_IDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;
    
    // Select CAN0 RX and TX in PIOA
    PIOA->PIO_ABSR &= ~(PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0);
    
    // Disable the Parallel IO (PIO) of the Rx and Tx pins so that the peripheral controller can use them
    PIOA->PIO_PDR = PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0;
    
    // Enable pull up on CANH and CANL pin
    PIOA->PIO_PUER = (PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0);    
    
    // Enable Clock for CAN0 in PMC
    // DIV = 1 (can clk = MCK/2), CMD = 1 (write), PID = 2B (CAN0)
    PMC->PMC_PCR = PMC_PCR_EN | (0/*??*/ << PMC_PCR_DIV_Pos) | PMC_PCR_CMD | (ID_CAN0 << PMC_PCR_PID_Pos); 
    PMC->PMC_PCER1 |= 1 << (ID_CAN0 - 32);
    
    //Set baudrate, Phase1, phase2 and propagation delay for can bus. Must match on all nodes!
    CAN0->CAN_BR = init.reg; 
    


    // Configure mailboxes
    // transmit
    CAN0->CAN_MB[txMailbox].CAN_MID = CAN_MID_MIDE;
    CAN0->CAN_MB[txMailbox].CAN_MMR = CAN_MMR_MOT_MB_TX;
    
    // receive
    CAN0->CAN_MB[rxMailbox].CAN_MAM = 0; // Accept all messages
    CAN0->CAN_MB[rxMailbox].CAN_MID = CAN_MID_MIDE;
    CAN0->CAN_MB[rxMailbox].CAN_MMR = CAN_MMR_MOT_MB_RX;
    CAN0->CAN_MB[rxMailbox].CAN_MCR |= CAN_MCR_MTCR;
    if(rxInterrupt){
        // Enable interrupt on receive
        CAN0->CAN_IER |= (1 << rxMailbox); 
        // Enable interrupt in NVIC 
        NVIC_EnableIRQ(ID_CAN0);
    }

    // Enable CAN
    CAN0->CAN_MR |= CAN_MR_CANEN;
}


void can_tx(CanMsg m){
    while(!(CAN0->CAN_MB[txMailbox].CAN_MSR & CAN_MSR_MRDY)){}
    
    // Set message ID and use CAN 2.0A protocol
    CAN0->CAN_MB[txMailbox].CAN_MID = CAN_MID_MIDvA(m.id) & ~CAN_MID_MIDE ;
        
    // Coerce maximum 8 byte length
    m.length = m.length > 8 ? 8 : m.length;
    
    //  Put message in can data registers
    CAN0->CAN_MB[txMailbox].CAN_MDL = m.dword[0];
    CAN0->CAN_MB[txMailbox].CAN_MDH = m.dword[1];
        
    // Set message length and mailbox ready to send
    CAN0->CAN_MB[txMailbox].CAN_MCR = (m.length << CAN_MCR_MDLC_Pos) | CAN_MCR_MTCR;
}

uint8_t can_rx(CanMsg* m){
    if(!(CAN0->CAN_MB[rxMailbox].CAN_MSR & CAN_MSR_MRDY)){
        return 0;
    }

    // Get message ID
    m->id = (uint8_t)((CAN0->CAN_MB[rxMailbox].CAN_MID & CAN_MID_MIDvA_Msk) >> CAN_MID_MIDvA_Pos);
        
    // Get data length
    m->length = (uint8_t)((CAN0->CAN_MB[rxMailbox].CAN_MSR & CAN_MSR_MDLC_Msk) >> CAN_MSR_MDLC_Pos);
    
    // Get data from CAN mailbox
    m->dword[0] = CAN0->CAN_MB[rxMailbox].CAN_MDL;
    m->dword[1] = CAN0->CAN_MB[rxMailbox].CAN_MDH;
                
    // Reset for new receive
    CAN0->CAN_MB[rxMailbox].CAN_MMR = CAN_MMR_MOT_MB_RX;
    CAN0->CAN_MB[rxMailbox].CAN_MCR |= CAN_MCR_MTCR;
    return 1;
}

// Example CAN interrupt handler
void CAN0_Handler(void){
	char can_sr = CAN0->CAN_SR;
	
	// RX interrupt
	if(can_sr & (1 << rxMailbox)){
		// Add your message-handling code here
		CanMsg can_msg;
		can_rx(&can_msg);
		//can_printmsg(can_msg);
		
		//CAN MSG HANDLER
		if (can_msg.id == CAN_ID_GAMEOVER){
			printf("GAMEOVER\n\r");
			gameloop = false;
		} else if (can_msg.id == CAN_ID_NEW_GAME) {
			printf("NEW GAME\n\r");
			gameloop = true;
			do_calib = true;
		} else if (can_msg.id == CAN_ID_GOAL) {
			//not covered by this node
		}
		else if(can_msg.id == CAN_ID_JOYSTICK){
			joy_pos_rec = (joy_pos_t){	.x = can_msg.signed_data[0],
										.y = can_msg.signed_data[1],
										.sl = can_msg.unsigned_data[2] - 127,
										.sr = can_msg.unsigned_data[3] - 127,
										.btn = can_msg.unsigned_data[4]};
		} else if (can_msg.id == CAN_ID_SETTINGS) {
			printf("SETTINGS: MODE: %d\n\r", can_msg.signed_data[0]);
			if(can_msg.signed_data[0] == MODE_1){
				controller.motor = &joy_pos_rec.x;
				controller.servo = &joy_pos_rec.y;
			} else if(can_msg.signed_data[0] == MODE_2){
				controller.motor = &joy_pos_rec.sl;
				controller.servo = &joy_pos_rec.sr;
			} else if(can_msg.signed_data[0] == MODE_3){
				controller.motor = &joy_pos_rec.sr;
				controller.servo = &joy_pos_rec.x;
			} else if(can_msg.signed_data[0] == MODE_4){
				controller.motor = &joy_pos_rec.x;
				controller.servo = &joy_pos_rec.x;
			} else if(can_msg.signed_data[0] == MODE_5){
				controller.motor = &joy_pos_rec.sr;
				controller.servo = &joy_pos_rec.sr;
			} else {
				printf("CAN0 message arrived in non-used mailbox\n\r");
			}
		}
	}
	
	if(can_sr & CAN_SR_MB0){
		// Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;
	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
}