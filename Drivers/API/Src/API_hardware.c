/*
 * API_hardware.c
 *
 *  Created on: Sep 18, 2025
 *      Author: ignaciocl
 */

#include "API_hardware.h"
#include "stm32f4xx_hal.h"

#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define B1_GPIO_Port GPIOC
#define B1_Pin GPIO_PIN_13

/**
 * @brief Sets the state of the LD2 GPIO pin.
 *
 * This function writes a logical HIGH or LOW to the LD2 LED pin.
 *
 * @param on Logical value to set:
 *            - true  → Turn LED on (set pin HIGH)
 *            - false → Turn LED off (set pin LOW)
 */
void setPin(bool_t on) {
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, on);
}

/**
 * @brief Sets the state of the LD2 GPIO pin.
 *
 * This function writes a logical HIGH or LOW to the LD2 LED pin.
 *
 * @param on Logical value to set:
 *            - true  → Turn LED on (set pin HIGH)
 *            - false → Turn LED off (set pin LOW)
 */
void togglePin() {
  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

/**
 * @brief Reads the state of the B1 GPIO pin.
 *
 * Reads the logical value from the B1 user button pin.
 *
 * @return true if the button pin is HIGH, false if it is LOW.
 */
bool_t readPin() {
	return HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
}

/**
 * @brief Returns the current system tick count.
 *
 * This function retrieves the number of milliseconds
 * since the system started, based on the HAL tick counter.
 *
 * @return The system uptime in milliseconds.
 */
uint32_t getTick() {
	return HAL_GetTick();
}

/**
 * @brief Introduces a blocking delay.
 *
 * @param delay Delay time in milliseconds.
 */
void blockingDelay(uint32_t delay) {
	HAL_Delay(delay);
}
