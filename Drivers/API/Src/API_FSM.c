/*
 * API_FSM.c
 *
 *  Created on: Oct 4, 2025
 *      Author: ignaciocl
 */

#include "API_LCD.h"
#include "API_scanner.h"
#include "API_debounce.h"
#include "API_delay.h"
#include "API_uart.h"
#include "common.h"
#include "messages.h"
static bool (*actualFunction)();
static bool idleStage();
static bool sendingStage();
static bool waitingStage();
static bool readingStage();
static bool writingStage();
static delay_t waitingTimeDelay;
static tick_t WAITING_TIME = 2000;

static uint16_t readValue = 0;
static uint8_t asciiValueRead[4] = { 0 };

/**
 * @brief Initializes the finite state machine and hardware components.
 *
 * Initializes UART, LCD, debounce FSM, and sets the starting state.
 *
 * @return true if initialization succeeded, false otherwise.
 */
bool Init_FSM() {
	actualFunction = idleStage;
	debounceFSM_init();
	bool uartInitialized = uartInit();
	bool initialized = InitLCD();
	if (initialized) {
		WriteString(ROW_ONE_WELCOME);
		GoToNextRow();
		WriteString(ROW_TWO_WELCOME);
		uartSendString(UART_WELCOME);
	} else if (uartInitialized) uartSendString(ERROR_INIT_UART);
	return initialized;
}

/**
 * @brief Updates the finite state machine.
 *
 * Executes the current FSM state function.
 *
 * @return true if the current state executed successfully, false otherwise.
 */
bool UpdateFSM() {
	return actualFunction();
}

/**
 * @brief Idle FSM stage.
 *
 * Waits for key input to transition to sending stage.
 *
 * @return true always, false is unused.
 */
bool idleStage() {
	debounceFSM_update();
	if (readKey()) {
		actualFunction = sendingStage;
	}
	return true;
}

/**
 * @brief FSM stage responsible for initiating sensor writing.
 *
 * Sends a write command to the sensor and transitions to waiting stage.
 *
 * @return true if successful, false otherwise.
 */
bool sendingStage() {
	bool status = WriteSensor();
	delayInit(&waitingTimeDelay, WAITING_TIME);
	status = status && ClearScreen() && WriteString(CALCULATING_DISTANCE);
	if (status) uartSendString(UART_CALCULATING_DISTANCE);
	actualFunction = waitingStage;
	return status;
}

/**
 * @brief FSM waiting stage.
 *
 * Waits for a predefined delay before proceeding to the reading stage.
 *
 * @return true always, false is unused.
 */
bool waitingStage() {
	if (delayRead(&waitingTimeDelay)) {
		actualFunction = readingStage;
	}
	return true;
}


/**
 * @brief FSM reading stage.
 *
 * Reads distance from sensor and converts it to ASCII format.
 *
 * @return true if successful, false otherwise.
 */
bool readingStage() {
	bool status = ReadSensor(&readValue);
	status = status && NumberToAscii(readValue, asciiValueRead);
	if (!status) {
		actualFunction = sendingStage; // Error in medition, fall back
	} else {
		actualFunction = writingStage;
	}
	return status;
}

/**
 * @brief FSM writing stage.
 *
 * Displays the measured distance on the LCD and sends it over UART.
 *
 * @return true if successful, false otherwise.
 */
bool writingStage() {
	bool status = ClearScreen();
	status = status && WriteString(READ_DISTANCE);
	status = status && GoToNextRow();
	status = status && WriteString(asciiValueRead) && WriteString(DISTANCE_UNIT);
	if (status) {
		status = uartSendString(UART_READ_DISTANCE) && uartSendString(asciiValueRead) && uartSendString(DISTANCE_UNIT);
		actualFunction = idleStage;
	}
	return status;
}
