#include "misc/utils.h"
#include "drivers/gamepad.h"
#include "misc/fonts.h"

// Function Prototypes
void SSD1308_Init(void);
void SSD1308_WriteCommand(uint8_t command);
void SSD1308_WriteData(uint8_t data);
void SSD1308_Clear(void);

// Initialize the SSD1308
void SSD1308_Init() {
	// Reset the display
	*(volatile uint8_t *)0x1000 = 0xAE; // Display off
	
	*(volatile uint8_t *)0x1000 = 0xA8; // Set multiplex ratio
	*(volatile uint8_t *)0x1000 = 0x3F; // Duty cycle
	*(volatile uint8_t *)0x1000 = 0xD3; // Set display offset
	*(volatile uint8_t *)0x1000 = 0x00; // No offset
	*(volatile uint8_t *)0x1000 = 0x40; // Set start line
	*(volatile uint8_t *)0x1000 = 0x20; // Set memory addressing mode
	*(volatile uint8_t *)0x1000 = 0x00; // Horizontal addressing mode
	*(volatile uint8_t *)0x1000 = 0xA1; // Set segment re-map
	*(volatile uint8_t *)0x1000 = 0xC8; // Set COM output scan direction
	*(volatile uint8_t *)0x1000 = 0xDA; // Set COM pins hardware configuration
	*(volatile uint8_t *)0x1000 = 0x12; // Alternative COM pins
	*(volatile uint8_t *)0x1000 = 0x81; // Set contrast
	*(volatile uint8_t *)0x1000 = 0xFF; // Contrast value
	*(volatile uint8_t *)0x1000 = 0xD9; // Set pre-charge period
	*(volatile uint8_t *)0x1000 = 0xF1; // Pre-charge period
	*(volatile uint8_t *)0x1000 = 0xDB; // Set VCOMH deselect level
	*(volatile uint8_t *)0x1000 = 0x40; // VCOMH level
	
	*(volatile uint8_t *)0x1000 = 0xAF; // Display on
}

void write_c(uint8_t data){
	volatile char *ext_mem = (char *) 0x1000;
	ext_mem[0x0000] = data;
}

// Write command to the SSD1308
void SSD1308_WriteCommand(uint8_t command) {
	*(volatile uint8_t *)0x1000 = command; // Write command
}

// Write data to the SSD1308
void SSD1308_WriteData(uint8_t data) {
	*(volatile uint8_t *)0x1200 = data; // Write data
}

// Clear the OLED display
void SSD1308_Clear() {
	for (uint8_t page = 0; page < 8; page++) {
		SSD1308_WriteCommand(0xB0 + page); // Set page address
		SSD1308_WriteCommand(0x00); // Set low column address
		SSD1308_WriteCommand(0x10); // Set high column address
		for (uint8_t col = 0; col < 128; col++) {
			SSD1308_WriteData(0x00); // Clear data
		}
	}
}

void oled_print_char(char c){
	for (int i=0; i<4; i++){
		int byte = pgm_read_byte(&font4[c][i]);
		SSD1308_WriteData(byte); // Clear data
	}
}

int main(void) {
	UART_init(MYUBRR);
	XMEM_init();
	SSD1308_Init();
	SSD1308_Clear();
	ADC_init();
	JOY_init();
	pos_t center = JOY_calibrate();
	int8_t x=0;
	int8_t y=0;
	
	oled_print_char(x/8+75);
	
	SSD1308_WriteCommand(0x2E);
	SSD1308_WriteCommand(0x26);
	SSD1308_WriteCommand(0x00);
	SSD1308_WriteCommand(0x00);
	SSD1308_WriteCommand(0x07);
	SSD1308_WriteCommand(0x2F);
	
	/*
	while(1){
		_delay_ms(10);
		pos_t pos = JOY_get_rel_pos(center);
		dir direction = JOY_get_dir(pos);
		switch(direction){
			case UP:
				y = (y <= 0) ? 0 : y-1;
				_delay_ms(80);
				break;
			case DOWN:
				y = (y >= 7) ? 7 : y+1;
				_delay_ms(80);
				break;
			case LEFT:
				x = (x <= 0) ? 0 : x-1;
				break;
			case RIGHT:
				x = (x >= 127) ? 127 : x+1;
				break;
			default:
				break;
		}
		uint8_t lower = x & 0x0F;
		uint8_t higher = (x & 0xF0) >> 4;
		
		oled_print_char(x/8+75);
	}
	
	*/

}
