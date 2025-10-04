/*
 * API_LCD.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ignaciocl
 */

#include "API_LCD.h"
#include "API_LCD_port.h"
#include "stm32f4xx_hal.h"

#define CLEAR_COMMAND 0x01
#define RETURN_HOME  0x02
static uint8_t CLEAR_COMMANDS[] = {
		CLEAR_COMMAND,
		RETURN_HOME
};
static uint8_t nextAddress = 0x00;
static uint8_t ROW_SIZE = 0x40;
static uint8_t MAX_ADDRESS = 0x80;
static uint8_t sentAddress;
static uint8_t FIRST_COMMAND = 0x30;
static uint8_t SECOND_COMMAND = 0x20;

#define LCD_CMD_FUNCTION_SET_4BIT   0x28  // 4-bit, 2-line, 5x8 font
#define LCD_CMD_DISPLAY_CONTROL     0x0C  // Display ON, Cursor OFF, Blink OFF
#define LCD_CMD_ENTRY_MODE_SET      0x06  // Increment, no shift
static uint8_t INIT_COMMANDS[] = {
    LCD_CMD_FUNCTION_SET_4BIT,
    LCD_CMD_DISPLAY_CONTROL,
    LCD_CMD_ENTRY_MODE_SET
};

static uint8_t initCommandsSize = sizeof(INIT_COMMANDS) / sizeof(INIT_COMMANDS[0]);
static uint8_t clearCommandsSize = sizeof(CLEAR_COMMANDS) / sizeof(CLEAR_COMMANDS[0]);

static bool GoToAddress(uint8_t address) {
	sentAddress = MAX_ADDRESS | address;

	return WriteLCD(&sentAddress, true);
}
bool WriteString(uint8_t* str) {
	return WriteLCD(str, false);
}
bool GoToNextRow() {
	nextAddress = (nextAddress + ROW_SIZE) % MAX_ADDRESS;
	return GoToAddress(nextAddress);
}


static bool set4Bit() {
	HAL_Delay(50); // Initial power-on delay

	if (!sendInitNibble(FIRST_COMMAND)) return false;
	HAL_Delay(5);

	if (!sendInitNibble(FIRST_COMMAND)) return false;
	HAL_Delay(1);

	if (!sendInitNibble(FIRST_COMMAND)) return false;
	HAL_Delay(1);

	if (!sendInitNibble(SECOND_COMMAND)) return false;  // Switch to 4-bit
	HAL_Delay(1);

	return true;
}

bool ClearScreen() {
	nextAddress = 0x00;
	for (uint8_t i = 0; i < clearCommandsSize; i++) {
		bool status = WriteLCD(&CLEAR_COMMANDS[i], true);
		if (!status) {
			return status;
		}
		HAL_Delay(2);
	}
	return GoToAddress(0x00);
}


bool InitLCD() {
	if (!set4Bit()) return false;

	for (uint8_t i = 0; i < initCommandsSize; i++) {
		if (!WriteLCD(&INIT_COMMANDS[i], true)) return false;
		HAL_Delay(2);
	}

	return ClearScreen();
}
