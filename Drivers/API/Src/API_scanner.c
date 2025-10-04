/*
 * API_scanner.c
 *
 *  Created on: Oct 3, 2025
 *      Author: ignaciocl
 */

#include "API_I2C.h"
#include <stddef.h>

static uint8_t ADDRESS = 0x70;
static uint8_t WRITE_COMMAND = 81;
#define READ_SIZE 2
static uint8_t readValue[READ_SIZE];

bool ReadSensor(uint8_t* valueRead) {
	if (valueRead == NULL) {
		return false;
	}
	ReceiveToSize(ADDRESS, readValue, READ_SIZE);
	*valueRead = (readValue[0] << 8) | readValue[1];
	return true;

}
bool WriteSensor() {
	return TransmitToSize(ADDRESS, &WRITE_COMMAND, 1);
}
