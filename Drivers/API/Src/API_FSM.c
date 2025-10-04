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
static uint16_t distanceInCM = 0;
static uint8_t readValue[2] = { 0 };
static uint8_t asciiValueRead[4] = { 0 };

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

bool UpdateFSM() {
	return actualFunction();
}

bool idleStage() {
	debounceFSM_update();
	if (readKey()) {
		actualFunction = sendingStage;
	}
	return true;
}

bool sendingStage() {
	bool status = WriteSensor();
	delayInit(&waitingTimeDelay, WAITING_TIME);
	status = status && ClearScreen() && WriteString(CALCULATING_DISTANCE);
	if (status) uartSendString(UART_CALCULATING_DISTANCE);
	actualFunction = waitingStage;
	return status;
}

bool waitingStage() {
	if (delayRead(&waitingTimeDelay)) {
		actualFunction = readingStage;
	}
	return true;
}

bool readingStage() {
	readValue[0] = '\0';
	readValue[1] = '\0';
	bool status = ReadSensor(readValue);
	distanceInCM = (readValue[0] << 8) | readValue[1];
	status = status && NumberToAscii(readValue, asciiValueRead);
	if (!status) {
		actualFunction = sendingStage; // Error in medition, fall back
	} else {
		actualFunction = writingStage;
	}
	return status;
}

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
