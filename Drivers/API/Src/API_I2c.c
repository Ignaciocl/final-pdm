/*
 * API_I2c.c
 *
 *  Created on: Sep 30, 2025
 *      Author: ignaciocl
 */

#include "API_I2C.h"

#include "stm32f4xx_hal.h"
extern I2C_HandleTypeDef hi2c1;
static const uint32_t TIMEOUT = 500;

/**
 * @brief Transmits a fixed-size message via I2C.
 *
 * @param unshiftedAddress I2C slave address (unshifted).
 * @param value Pointer to the data to send.
 * @return true if transmission succeeded, false otherwise.
 */
bool Transmit(uint16_t unshiftedAddress, uint8_t* value) {
	return HAL_I2C_Master_Transmit (&hi2c1, unshiftedAddress<<1, value, sizeof(value), TIMEOUT) == HAL_OK;
}

/**
 * @brief Transmits data of a specified size via I2C.
 *
 * @param unshiftedAddress I2C slave address (unshifted).
 * @param value Pointer to the data to send.
 * @param size Number of bytes to transmit.
 * @return true if transmission succeeded, false otherwise.
 */
bool TransmitToSize(uint16_t unshiftedAddress, uint8_t* value, uint16_t size) {
	return HAL_I2C_Master_Transmit (&hi2c1, unshiftedAddress<<1, value, size, TIMEOUT) == HAL_OK;
}

/**
 * @brief Receives data of a specified size via I2C.
 *
 * @param unshiftedAddress I2C slave address (unshifted).
 * @param value Pointer to the buffer where data will be stored.
 * @param size Number of bytes to receive.
 * @return true if reception succeeded, false otherwise.
 */
bool ReceiveToSize(uint16_t unshiftedAddress, uint8_t* value, uint16_t size) {
	return HAL_I2C_Master_Receive (&hi2c1, unshiftedAddress<<1, value, size, TIMEOUT) == HAL_OK;
}
