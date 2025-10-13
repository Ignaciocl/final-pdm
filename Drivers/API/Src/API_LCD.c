/*
 * API_LCD.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ignaciocl
 */

#include "API_LCD.h"
#include "API_LCD_port.h"
#include "API_hardware.h"

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

/**
 * @brief Sets the LCD cursor to a specific address.
 *
 * @param address LCD address to move to.
 * @return true if successful, false otherwise.
 */
static bool goToAddress(uint8_t address) {
	sentAddress = MAX_ADDRESS | address;

	return WriteLCD(&sentAddress, true);
}

/**
 * @brief Writes a string to the LCD display.
 *
 * @param str Pointer to the null-terminated string to display.
 * @return true if successful, false otherwise.
 */
bool WriteString(uint8_t* str) {
	return WriteLCD(str, false);
}

/**
 * @brief Moves the LCD cursor to the next row.
 *
 * @return true if successful, false otherwise.
 */
bool GoToNextRow() {
	nextAddress = (nextAddress + ROW_SIZE) % MAX_ADDRESS;
	return goToAddress(nextAddress);
}

/**
 * @brief Initializes the LCD to operate in 4-bit mode.
 *
 * Sends the required initialization sequence for 4-bit communication.
 *
 * @return true if initialization succeeded, false otherwise.
 */
static bool set4Bit() {
	blockingDelay(50); // Initial power-on delay

	if (!sendInitNibble(FIRST_COMMAND)) return false;
	blockingDelay(5);

	if (!sendInitNibble(FIRST_COMMAND)) return false;
	blockingDelay(1);

	if (!sendInitNibble(FIRST_COMMAND)) return false;
	blockingDelay(1);

	if (!sendInitNibble(SECOND_COMMAND)) return false;  // Switch to 4-bit
	blockingDelay(1);

	return true;
}

/**
 * @brief Clears the LCD screen and resets the cursor to address 0x00.
 *
 * @return true if clear operation succeeded, false otherwise.
 */
bool ClearScreen() {
	nextAddress = 0x00;
	for (uint8_t i = 0; i < clearCommandsSize; i++) {
		bool status = WriteLCD(&CLEAR_COMMANDS[i], true);
		if (!status) {
			return status;
		}
		blockingDelay(2);
	}
	return goToAddress(0x00);
}

/**
 * @brief Initializes the LCD display.
 *
 * Performs the full LCD setup sequence and clears the screen.
 *
 * @return true if initialization succeeded, false otherwise.
 */
bool InitLCD() {
	if (!set4Bit()) return false;

	for (uint8_t i = 0; i < initCommandsSize; i++) {
		if (!WriteLCD(&INIT_COMMANDS[i], true)) return false;
		blockingDelay(2);
	}

	return ClearScreen();
}
