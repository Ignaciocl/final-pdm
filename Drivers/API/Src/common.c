/*
 * common.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ignaciocl
 */

#include "common.h"
#include <stddef.h>
/**
 * @brief Finds the index of a specific character in a string or the end of the chain.
 *
 * Iterates over a byte string until the specified character is found.
 * If the character is not found within the maximum number of iterations,
 * the function calls @ref Error_Handler.
 *
 * @param[in] pstring      Pointer to the input string (null-terminated).
 * @param[in] charToFind   Character to search for.
 *
 * @return Index of the found character within the string.
 */
uint16_t findCharIndex(uint8_t *pstring, uint8_t charToFind) {
	uint16_t i = 0;
	uint8_t actual = pstring[i];
	while (actual != charToFind && actual != '\0') {
		i++;
		actual = pstring[i];
	}
	return i;
}


bool NumberToAscii(uint8_t *value, uint8_t *buffer) {
    if (value == NULL || buffer == NULL) return false;

    uint16_t number;

    // Auto-detect whether the number uses 1 or 2 bytes
    // (If second byte is 0, assume 1-byte number)
    if (value[1] == 0x00) {
        number = value[0];
    } else {
        number = ((uint16_t)value[0] << 8) | value[1];
    }

    // Enforce 1–9999 range
    if (number < 1 || number > 9999) {
        return false;
    }

    uint8_t i = 0;

    // Thousands
    if (number >= 1000) {
        buffer[i++] = (number / 1000) + '0';
        number %= 1000;
    }

    // Hundreds
    if (number >= 100 || i > 0) {
        buffer[i++] = (number / 100) + '0';
        number %= 100;
    }

    // Tens
    if (number >= 10 || i > 0) {
        buffer[i++] = (number / 10) + '0';
        number %= 10;
    }

    // Ones
    buffer[i++] = number + '0';

    buffer[i] = '\0';  // Null-terminate
    return true;
}
