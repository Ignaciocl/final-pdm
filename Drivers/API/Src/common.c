/*
 * common.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ignaciocl
 */

#include "common.h"
#include <stddef.h>
#include <stdio.h>
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

/**
 * @brief Converts a 16-bit unsigned integer to its ASCII string representation.
 *
 * @param value The number to convert.
 * @param buffer Pointer to the buffer where the ASCII result will be stored.
 * @return true if conversion was successful, false if buffer is NULL.
 */
bool NumberToAscii(uint16_t value, uint8_t *buffer) {
    if (buffer == NULL) return false;

    // Convert the number (uint16_t) to string (ASCII)
    sprintf((char*)buffer, "%u", value);

    return true;
}
