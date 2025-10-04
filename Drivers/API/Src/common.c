/*
 * common.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ignaciocl
 */

#include "common.h"

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
