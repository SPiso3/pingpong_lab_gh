#include "utils.h"

extern FILE *oled_output;

void test_UART(){
	UART_init(MYUBRR);
	printf("\nProva trasmissione\n\r");
	int n = 0;
	while(1){
		printf("%d\n\r",n++);
		_delay_ms(200);
	}
}

void test_SRAM(void)
{	
	XMEM_init();
	UART_init(MYUBRR);
	volatile char *ext_ram = (char *) SRAM_BASE_ADDR; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n\r");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			//printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			//printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n\r%4d errors in write phase and \n\r%4d errors in retrieval phase\n\r\n\r", write_errors, retrieval_errors);
}

void test_XMEM(void){
	XMEM_init();
	UART_init(MYUBRR);
	volatile char *ext_mem = (char *) XMEM_BASE_ADDR;
	
	printf("\nOLED ZONE...\n\r");
	_delay_ms(2000);
	for(uint16_t addr = 0x100; addr<0x3FF; addr += 0x02){
		ext_mem[addr] = 0xAB;
		uint8_t ret_val = ext_mem[addr];
		printf("addr[%4x] = %02X\n\r", addr, ret_val);
	}
	
	printf("\nADC ZONE...\n\r");
	_delay_ms(2000);
	for(uint16_t addr = 0x400; addr<0x7FF; addr += 0x02){
		ext_mem[addr] = 0xAB;
		uint8_t ret_val = ext_mem[addr];
		printf("addr[%4x] = %02X\n\r", addr, ret_val);
	}
	
	printf("\nSRAM ZONE...\n\r");
	_delay_ms(2000);
	for(uint16_t addr = 0x800; addr<0xFFF; addr += 0x02){
		ext_mem[addr] = 0xAB;
		uint8_t ret_val = ext_mem[addr];
		printf("addr[%4x] = %02X\n\r", addr, ret_val);
	}
}

void test_ADC(){
	UART_init(MYUBRR);
	XMEM_init();
	ADC_init();
	
	while(1){
		ADC_sample4();
		
		printf("\033[2J\033[H");
		printf("vertical %d\n\r",ADC_read());
		printf("horizont %d\n\r",ADC_read());
		printf("slider L %d\n\r",ADC_read());
		printf("slider R %d\n\r",ADC_read());
		_delay_ms(100);
	}
}

void test_JOYSTICK(){
	UART_init(MYUBRR);
	XMEM_init();
	ADC_init();
	
	_delay_ms(1000);
	JOY_init();
	_delay_ms(1000);
	
	while(1){
		pos_t pos = JOY_get_rel_pos();
		dir direction = JOY_get_dir();
		sliders_t sliders = JOY_get_sliders();
		
		printf("\033[2J\033[H");
		printf("x %d\n\r",pos.x);
		printf("y %d\n\r",pos.y);
		
		printf("DIR: %d\n\r",direction);
		
		printf("sliderL %d\n\r",sliders.left);
		printf("sliderR %d\n\r",sliders.right);
		
		printf("BTN: %d\n\r",(PIND & (1<<JOY_BUTTON))>>JOY_BUTTON);
		
		_delay_ms(100);
	}
}

void test_OLED(){
	UART_init(MYUBRR);
	XMEM_init();
	OLED_init();
	OLED_reset();
	
	for(int p=0; p<8; p++){
		OLED_goto_pos(p,p*8);
		for(int c=0; c<8; c++){
			OLED_write_data(0xFF);
			_delay_ms(50);
		}
	}
	
	OLED_goto_pos(7,128-8);
	for(int c=0; c<8; c++)
		OLED_write_data(0xFF);
	_delay_ms(100);
	
	OLED_goto_pos(0,128-8);
	for(int c=0; c<8; c++)
	OLED_write_data(0xFF);
	_delay_ms(100);
	
	for(uint8_t i=0; i<4; i++){
		_delay_ms(200);
		OLED_invert();
	}
	
	OLED_reset();
	fprintf(oled_output, "prova ...");
	_delay_ms(1000);
	OLED_reset();
	
	OLED_reset();
	char value = ' ';
	while(value < ' ' + 95){
		fprintf(oled_output, "%c",value++);
		_delay_ms(50);
	}
}

void test_MENU(){
	UART_init(MYUBRR);
	XMEM_init();
	ADC_init();
	JOY_init();
	
	OLED_init();
	OLED_reset();
	
	menu_init();
}

void test_MCP_loopback(){
	UART_init(MYUBRR);
	MCP_init(MODE_LOOPBACK); // set loopback mode
	
	//send
	MCP_write(MCP_TXB0SIDH, 0xab);
	MCP_request_to_send(0);
	//interrupt has raised!
	
	//check flag
	uint8_t flag = (MCP_read(MCP_CANINTF) & MCP_RX0IF);
	printf("interrupt flag: %x\r\n", flag);
	
	//read data
	uint8_t byte = MCP_read(MCP_RXB0SIDH);
	printf("received: %x\r\n", byte);
	
	//clear flag
	MCP_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
	flag = (MCP_read(MCP_CANINTF) & MCP_RX0IF);
	printf("is it CLEARED?: int flag: %x\r\n", flag);
}

void test_LB_CAN_main(){
	UART_init(MYUBRR);
	MCP_init(MODE_NORMAL); // set loopback mode
	XMEM_init();
	ADC_init();
	JOY_init();
	
	cli();
	GICR |= (1 << INT0);					// Enable INT0
	MCUCR &= ~((1 << ISC01)|(0 << ISC00));	// mode:00 = trigger when LOW
	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2);
	sei();
	
	while(1){
		_delay_ms(100);
		printf("\033[2J\033[H");
		//create data
		pos_t pos = JOY_get_rel_pos();
		printf("JOY_16t: %x %x\r\n",sizeof(pos.x), pos.y);
		int8_t xl = (int8_t)pos.x;
		int8_t yl  = (int8_t)pos.y;
		printf("JOY_8t: %d %d\r\n",xl, yl);
		message_t msg = {0xAB, 2, .signed_data={xl,yl}};
	
		//check CAN RX flag before sending (should be 0)
		uint8_t flag = (MCP_read(MCP_CANINTF) & MCP_RX0IF);
		printf("interrupt flag: %x\r\n", flag);
	
		//send
	    printf("sending message...\r\n");
		CAN_send(&msg); //this will trigger the interrupt... (because of LOOPBACK)
		break;
	}
}

void test_LB_CAN_isr(){
	printf("INTERRUPT on INT0!\n\r");
	
	//check CAN RX flag (should be 1)
	uint8_t flag = (MCP_read(MCP_CANINTF) & MCP_RX0IF);
	printf("interrupt flag: %x\r\n", flag);
	
	//read data
	message_t rec = CAN_receive();
	printf("received CAN MESSAGE = ID: %x LENGTH: %d DATA: ", rec.id, rec.length);
	
	for(int i=0; i<rec.length; i++){
		printf("%d",rec.signed_data[i]);
	}
	printf("\n\r");
	
	//clear flag
	MCP_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
	
	//check CAN RX flag (should be 0)
	flag = (MCP_read(MCP_CANINTF) & MCP_RX0IF);
	printf("interrupt flag: %x\r\n", flag);
}