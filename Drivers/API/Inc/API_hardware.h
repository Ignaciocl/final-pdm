/*
 * API_hardware.h
 *
 *  Created on: Sep 18, 2025
 *      Author: ignaciocl
 */

#ifndef API_INC_API_HARDWARE_H_
#define API_INC_API_HARDWARE_H_
#include <stdbool.h>
#include <stdint.h>
typedef uint32_t tick_t;
typedef bool bool_t;

void setPin(bool_t on);
void togglePin();
bool_t readPin();
uint32_t getTick();

#endif /* API_INC_API_HARDWARE_H_ */
