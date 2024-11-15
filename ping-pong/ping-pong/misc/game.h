#ifndef GAME_H_
#define GAME_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../drivers/include/UART.h"
#include "../drivers/include/XMEM.h"
#include "../drivers/include/ADC.h"
#include "../drivers/include/gamepad.h"
#include "../drivers/include/OLED.h"
#include "../drivers/include/SPI.h"
#include "../drivers/include/MCP.h"
#include "../drivers/include/CAN.h"

#include <util/delay.h>

//------------------------------------------
//DEFINES

#define JOY_SEND_TRESHOLD 5
#define SLIDER_SEND_TRESHOLD 12

#define MAXLIVES 10

#define CALIB_TIME 3000 //ms
#define FPS 60
#define GAME_SPEED 1000/FPS

#define MAX(a,b) ((a >= b) ? a : b)
#define MIN(a,b) ((a <= b) ? a : b)

//settings for playing the game, described in node2 by the sam typedef
typedef enum {
	MODE_1,
	MODE_2,
	MODE_3,
	MODE_4,
	MODE_5,
} Settings;

//------------------------------------------
//functions()

//void test_UART();
void test_SRAM();
//void test_XMEM();
//void test_ADC();
//void test_JOYSTICK();
void test_OLED();
//void test_MENU();
//void test_MCP_loopback();
//void test_LB_CAN_main();
//void test_LB_CAN_isr();

//send settings to node2
void settings(uint8_t mode);
void settings_mode1();
void settings_mode2();
void settings_mode3();
void settings_mode4();
void settings_mode5();

//send new game() to node2
void new_game();

#endif /* GAME_H_ */