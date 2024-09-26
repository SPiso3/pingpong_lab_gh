#include "utils.h"

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
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x900;
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
	for(uint16_t addr = 0x000; addr<0x3FF; addr += 0x02){
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
		
		printf("\x1B[2J\x1b[H");
		printf("vertical %d\n\r",ADC_read());
		printf("horizont %d\n\r",ADC_read());
		printf("slider L %d\n\r",ADC_read());
		printf("slider R %d\n\r",ADC_read());
		_delay_ms(100);
	}
}	