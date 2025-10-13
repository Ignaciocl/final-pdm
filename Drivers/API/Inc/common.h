/*
 * common.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ignaciocl
 */

#ifndef API_INC_COMMON_H_
#define API_INC_COMMON_H_

#include <stdint.h>
#include <stdbool.h>
uint16_t findCharIndex(uint8_t *pstring, uint8_t charToFind);
bool NumberToAscii(uint16_t value, uint8_t *buffer);
#endif /* API_INC_COMMON_H_ */
