/*
 * API_uart.c
 *
 *  Created on: Sep 25, 2025
 *      Author: ignaciocl
 */

#include "API_uart.h"
#include "stm32f4xx_hal.h"
#include "string.h"
#include <stdio.h>

static UART_HandleTypeDef UartHandle;
void Error_Handler(void); // Implemented in main.c always and defined in its main.h

const uint32_t TIMEOUT = 3000;
static void errorIfNeeded(void* delay) {
	if (delay == NULL) {
		Error_Handler();
	}
}

/**
 * @brief Finds the index of a specific character in a string.
 *
 * Iterates over a byte string until the specified character is found.
 * If the character is not found within the maximum number of iterations,
 * the function calls @ref Error_Handler.
 *
 * @param[in] pstring      Pointer to the input string (null-terminated).
 * @param[in] charToFind   Character to search for.
 * @param[in] max          Maximum number of characters to inspect.
 *
 * @return Index of the found character within the string.
 */
static uint16_t findCharIndex(uint8_t *pstring, uint8_t charToFind, uint16_t max) {
	uint16_t i = 0;
	uint8_t actual = pstring[i];
	while (actual != charToFind) {
		i++;
		actual = pstring[i];
		if (i >= max) {
			Error_Handler();
		}
	}
	return i;
}

/**
 * @brief Initializes the UART2 peripheral.
 *
 * Configures the UART with the following settings:
 * - Baud rate: 9600
 * - Word length: 9 bits
 * - Stop bits: 1
 * - Parity: Odd
 * - Hardware flow control: None
 * - Mode: TX/RX
 * - Oversampling: 16
 *
 * After initialization, it sends a JSON string over UART describing
 * the configuration.
 *
 * @return `true` if the UART was successfully initialized,
 *         `false` otherwise.
 */
bool_t uartInit() {
	UartHandle.Instance = USART2;
	UartHandle.Init.BaudRate = 9600;
	UartHandle.Init.WordLength = UART_WORDLENGTH_9B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_ODD;
	UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	bool_t status = HAL_UART_Init(&UartHandle) == HAL_OK;
	if (status) {
		char uart_json[256];

		snprintf(uart_json, sizeof(uart_json),
			"\r\033[2J{"
			"\"BaudRate\":%ld,"
			"\"WordLength\":\"%ld\","
			"\"StopBits\":\"%ld\","
			"\"Parity\":\"%ld\","
			"\"HwFlowCtl\":\"%ld\","
			"\"Mode\":\"%ld\","
			"\"Instance\":\"%s\""
			"}\n",
			UartHandle.Init.BaudRate,
			UartHandle.Init.WordLength,
			UartHandle.Init.StopBits,
			UartHandle.Init.Parity,
			UartHandle.Init.HwFlowCtl,
			UartHandle.Init.Mode,
			"USART2"
		);
		uint8_t i = findCharIndex((uint8_t*)uart_json, '\n', 256);
		uartSendStringSize((uint8_t*)uart_json, i+1);
	};
	return status;
}
/**
 * @brief Sends a null-terminated string over UART.
 *
 * The function determines the string length by searching for the
 * null terminator (`'\0'`) before transmitting.
 *
 * @param[in] pstring  Pointer to the null-terminated string to transmit.
 */
void uartSendString(uint8_t * pstring) {
	errorIfNeeded(pstring);
	uint16_t max = 0;
	uint16_t i = findCharIndex(pstring, '\0', max -1);
	HAL_UART_Transmit(&UartHandle, pstring, i, TIMEOUT);
}
/**
 * @brief Sends a fixed-size string over UART.
 *
 * Transmits exactly @p size bytes from the given string buffer.
 *
 * @param[in] pstring  Pointer to the string buffer to transmit.
 * @param[in] size     Number of bytes to transmit.
 */
void uartSendStringSize(uint8_t * pstring, uint16_t size) {
	errorIfNeeded(pstring);
	HAL_UART_Transmit(&UartHandle, pstring, size, TIMEOUT);
}

/**
 * @brief Receives a fixed-size string over UART.
 *
 * Reads exactly @p size bytes from UART into the given buffer.
 *
 * @param[out] pstring  Pointer to the buffer where received data will be stored.
 * @param[in]  size     Number of bytes to receive.
 */
void uartReceiveStringSize(uint8_t * pstring, uint16_t size) {
	errorIfNeeded(pstring);
	HAL_UART_Receive(&UartHandle, pstring, size, TIMEOUT);
}


