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

/**
 * @brief Reads a 16-bit value from the connected sensor via I2C.
 *
 * @param valueRead Pointer to store the read value.
 * @return true if read was successful, false otherwise.
 */
bool ReadSensor(uint16_t* valueRead) {
	if (valueRead == NULL) {
		return false;
	}
	ReceiveToSize(ADDRESS, readValue, READ_SIZE);
	*valueRead = (uint16_t) readValue[0] << 8 | readValue[1];
	return true;
}

/**
 * @brief Sends a predefined command to the sensor.
 *
 * @return true if transmission succeeded, false otherwise.
 */
bool WriteSensor() {
	return TransmitToSize(ADDRESS, &WRITE_COMMAND, 1);
}
