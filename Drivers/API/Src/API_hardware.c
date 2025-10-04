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

void setPin(bool_t on) {
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, on);
}
void togglePin() {
  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}
bool_t readPin() {
	return HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
}
uint32_t getTick() {
	return HAL_GetTick();
}
