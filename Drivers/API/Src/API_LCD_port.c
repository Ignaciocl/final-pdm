/*
 * API_LCD_port.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ignaciocl
 */

#include "API_LCD_port.h"
#include "API_I2C.h"
#include "common.h"
static const uint16_t ADDRESS = 0x27;
#define LCD_BACKLIGHT  0x08
#define LCD_ENABLE     0x04
#define LCD_RW         0x00
#define LCD_RS         0x01

static const uint8_t MAX_LCD_CHARS = 16;  // More than this will fill the row
static const uint8_t MAX_LCD_BYTES = (MAX_LCD_CHARS * 4);

/**
 * @brief Converts a sequence of bytes into LCD-compatible 4-bit commands.
 *
 * @param value Pointer to input data (characters or commands).
 * @param size Pointer to size of the input; updated to output size after conversion.
 * @param lcdBuffer Pointer to buffer where converted bytes will be stored.
 * @param command true if value represents a command, false if data (string).
 */
static void translateIntoLcd(uint8_t* value, uint16_t* size, uint8_t* lcdBuffer, bool command) {
	uint16_t outIndex = 0;
	uint8_t rsFlag = command ? 0x00 : LCD_RS;

	for (uint16_t i = 0; i < *size; i++) {
		uint8_t ch = value[i];

		uint8_t highNibble = ch & 0xF0;
		uint8_t lowNibble  = (ch << 4) & 0xF0;

		// Send high nibble
		lcdBuffer[outIndex++] = highNibble | LCD_BACKLIGHT | rsFlag | LCD_ENABLE;
		lcdBuffer[outIndex++] = highNibble | LCD_BACKLIGHT | rsFlag;

		// Send low nibble
		lcdBuffer[outIndex++] = lowNibble | LCD_BACKLIGHT | rsFlag | LCD_ENABLE;
		lcdBuffer[outIndex++] = lowNibble | LCD_BACKLIGHT | rsFlag;
	}
	*size = outIndex;
}


/**
 * @brief Sends an LCD initialization nibble.
 *
 * @param nibble The upper 4 bits of the command to send.
 * @return true if transmission succeeded, false otherwise.
 */
bool sendInitNibble(uint8_t nibble) {
	uint8_t highNibble = nibble & 0xF0;  // Extract upper 4 bits

	uint8_t data[2];
	data[0] = highNibble | LCD_BACKLIGHT | LCD_ENABLE;
	data[1] = highNibble | LCD_BACKLIGHT;

	return TransmitToSize(ADDRESS, data, 2);
}

/**
 * @brief Writes data or command bytes to the LCD over I2C.
 *
 * @param value Pointer to data or command buffer.
 * @param command true if sending a command, false if writing text.
 * @return true if successful, false otherwise.
 */
bool WriteLCD(uint8_t* value, bool command) {
    uint16_t size = command ? 1 : findCharIndex(value, '\0');
    if (size > MAX_LCD_CHARS) {
    	size = MAX_LCD_CHARS;
    }
    uint8_t lcdBuffer[MAX_LCD_BYTES] = { };
    translateIntoLcd(value, &size, lcdBuffer, command);

    bool success = TransmitToSize(ADDRESS, lcdBuffer, size);
    return success;
}

