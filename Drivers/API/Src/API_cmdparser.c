/*
 * API_cmdparser.c
 *
 *  Created on: Sep 27, 2025
 *      Author: ignaciocl
 */

#include "API_uart.h"
#include "API_delay.h"
#include "API_hardware.h"
#include <stddef.h>
#include <string.h>

#define CMD_MAX_LINE 64       // bytes, incluye terminador '\0'
#define CMD_MAX_TOKENS 5      // COMANDO + hasta 4 args
typedef enum {
  CMD_OK = 0,
  CMD_ERR_TIMEOUT,
  CMD_ERR_OVERFLOW,
  CMD_ERR_SYNTAX,
  CMD_ERR_UNKNOWN,
  CMD_ERR_ARG
} cmd_status_t;
static const uint8_t END_LINE_CHARS[] = { '\n', '\r' };
void Error_Handler();
static uint8_t command[CMD_MAX_LINE];
static uint8_t pointer = 0;
static uint8_t OVERFLOW_ERROR[] = "\r\nERROR: line too long";
static uint8_t TIMEOUT_ERROR[] = "\r\nERROR: timeout";
static uint8_t ARGS_ERROR[] = "\r\nERROR: invalid amount of parameters";
static uint8_t INVALID_COMMAND_ERROR[] = "\r\nERROR: invalid command";
static uint8_t INIT_MESSAGE[] = "\r\n> ";
static uint8_t OK_MESSAGE[] = "\r\nOK";
static uint8_t LED_COMMAND[] = "led";
static uint8_t LED_ON_COMMAND[] = "on";
static uint8_t LED_TOGGLE_COMMAND[] = "toggle";
static uint8_t LED_OFF_COMMAND[] = "off";
static uint8_t COMMENT_1 = '#';
static uint8_t COMMENT_2 = '/';

static uint8_t* ERROR_MESSAGE;
static delay_t timeoutDelay;
static delay_t messageDelay;
static const tick_t MESSAGE_DELAY = 5000;
static const tick_t TIMEOUT_DELAY = 10000;
static uint8_t* receivedCommand;
static uint8_t* argZero;
static uint8_t* argOne;
static bool_t firstIdle = true;
static bool_t commentLine = false;

static void (*actualFunction)();
static void idleState();
static void receiveState();
static void timeoutState();
static void parseState();
static void execState();
static void errorState();

/**
 * @brief Clears the command buffer.
 *
 * Resets all characters in the global @ref command buffer
 * to the null character ('\0').
 */
static void clearCommand() {
	uint8_t i = 0;
	while (i < 64) {
		command[i++] = '\0';
	}
}

/**
 * @brief Checks if a character is an end-of-line marker.
 *
 * Determines whether the given character matches any of the
 * characters defined in @ref END_LINE_CHARS.
 *
 * @param[in] mander  Character to check.
 * @return `true` if the character is an end-of-line marker,
 *         `false` otherwise.
 */
static bool_t isEndCharacter(uint8_t mander) {
	uint8_t i = 0;
	uint8_t size = sizeof(END_LINE_CHARS) / sizeof(END_LINE_CHARS[0]);
	while (i < size) {
		if (mander == END_LINE_CHARS[i]) {
			return true;
		}
		i++;
	}
	return false;
}

/**
 * @brief Validates whether a character is allowed in the command buffer.
 *
 * Accepted characters include:
 * - Newline (`10`)
 * - Carriage return (`13`)
 * - Null (`0`)
 * - Printable ASCII characters (`32`–`125`)
 *
 * @param[in] mander  Character to validate.
 * @return `true` if the character is valid,
 *         `false` otherwise.
 */
static bool_t isValidChar(uint8_t mander) {
	return mander == 10 || mander == 13 || mander == 0 || (mander > 31 && mander < 126);
}

/**
 * @brief Writes a character into the command buffer.
 *
 * Stores the given character at the current @ref pointer position
 * in the @ref command buffer, then increments the pointer.
 *
 * @param[in] mander  Character to store.
 */
static void writeChar(uint8_t mander) {
	command[pointer++] = mander;
}

/**
 * @brief FSM idle state handler.
 *
 * - Sends an initialization message on the first idle call.
 * - Receives characters from UART one by one.
 * - Detects comment lines and end-of-line characters.
 * - Transitions to:
 *   - @ref receiveState if valid input starts a command,
 *   - Stays in idle state if invalid or comment input is received.
 */
static void idleState() {
	if (firstIdle) {
		uartSendString(INIT_MESSAGE);
		firstIdle = false;
	}
	uint8_t mander = '\0';
	uartReceiveStringSize(&mander, 1);
	if (!isValidChar(mander)) {
		return;
	}
	if (mander == COMMENT_1 || mander == COMMENT_2) {
		commentLine = true;
	}
	if (!commentLine && !isEndCharacter(mander) && mander != '\0') {
		uartSendStringSize(&mander, 1);
		clearCommand();
		actualFunction = receiveState;
		pointer = 0;
		writeChar(mander);
		delayInit(&timeoutDelay, TIMEOUT_DELAY);
		firstIdle = true;
	} else if (commentLine) {
		uartSendStringSize(&mander, 1);
		commentLine = !isEndCharacter(mander);
		firstIdle = !commentLine;
	}
}

/**
 * @brief FSM receive state handler.
 *
 * - Receives characters into the command buffer.
 * - Echoes received characters via UART.
 * - Transitions to:
 *   - @ref parseState if an end-of-line character is received,
 *   - @ref errorState on buffer overflow,
 *   - @ref timeoutState if no input arrives before timeout.
 */
static void receiveState() {
	uint8_t mander = '\0';
	uartReceiveStringSize(&mander, 1);
	uartSendStringSize(&mander, 1);
	if (isEndCharacter(mander)) {
		actualFunction = parseState;
	} else if (mander != '\0') {
		writeChar(mander);
		if (pointer >= CMD_MAX_LINE) {
			ERROR_MESSAGE = OVERFLOW_ERROR;
			actualFunction = errorState;
		}
	} else if (delayRead(&timeoutDelay)) {
		actualFunction = timeoutState;
	}
}

/**
 * @brief FSM error state handler.
 *
 * - Sends the current @ref ERROR_MESSAGE over UART (if set).
 * - Waits for a delay before resetting back to @ref idleState.
 * - Clears the error message after transmission.
 */
static void errorState() {
	if (ERROR_MESSAGE != NULL) {
		delayInit(&messageDelay, MESSAGE_DELAY);
		uartSendString(ERROR_MESSAGE);
	}
	if (delayRead(&messageDelay)) {
		actualFunction = idleState;
	}
	ERROR_MESSAGE = NULL;
}

/**
 * @brief FSM parse state handler.
 *
 * - Splits the command buffer into tokens separated by spaces.
 * - Converts all command characters to lowercase.
 * - Validates token count and arguments.
 * - Transitions to:
 *   - @ref execState if a valid command is parsed,
 *   - @ref errorState otherwise.
 */
static void parseState() {
	uint8_t *tokens[CMD_MAX_TOKENS] = {0};
	uint8_t tokenCount = 0;

	uint8_t *p = command;
	while (*p && tokenCount < pointer + 1) {
		while (*p == ' ') p++;
		if (!*p) break;

		tokens[tokenCount++] = p;
		if (tokenCount >= CMD_MAX_TOKENS) {
			ERROR_MESSAGE = ARGS_ERROR;
			break;
		}

		while (*p && *p != ' ') {
			if (*p >= 'A' && *p <= 'Z') {
				*p = *p + ('a' - 'A');
			}
			p++;
		}

		if (*p == ' ') {
			*p = '\0';
			p++;
		}
	}
	if (tokenCount == 0) {
		ERROR_MESSAGE = ARGS_ERROR;
	}
	if (tokenCount > 0 && tokenCount < CMD_MAX_TOKENS) {
		actualFunction = execState;
		receivedCommand = tokens[0];
		argZero = tokens[1];
		argOne = tokens[2];
	} else {
		actualFunction = errorState;
	}
}

/**
 * @brief FSM execution state handler.
 *
 * - Executes the parsed command (e.g., LED control commands).
 * - Sends an acknowledgment or error message over UART.
 * - Transitions back to @ref idleState after a delay.
 */
static void execState() {
	if (receivedCommand != NULL) {
		delayInit(&messageDelay, MESSAGE_DELAY);
		bool_t invalidCommand = true;
		if (!strcmp((char*)receivedCommand, (char*)LED_COMMAND)) {
			char* arg = (char*)argZero;
			if (!strcmp(arg, (char*)LED_ON_COMMAND)) {
				setPin(true);
				invalidCommand = false;
			}
			else if (!strcmp(arg, (char*)LED_TOGGLE_COMMAND)) {
				togglePin();
				invalidCommand = false;
			}
			else if (!strcmp(arg, (char*)LED_OFF_COMMAND)) {
				setPin(false);
				invalidCommand = false;
			}
		}
		if (invalidCommand) {
			ERROR_MESSAGE = INVALID_COMMAND_ERROR;
			actualFunction = errorState;
		} else {
			uartSendString(OK_MESSAGE);
		}

	}
	if (delayRead(&messageDelay)) {
		actualFunction = idleState;
	}
	receivedCommand = NULL;
}

/**
 * @brief FSM timeout state handler.
 *
 * Sets @ref ERROR_MESSAGE to a timeout error and transitions
 * to @ref errorState.
 */
static void timeoutState() {
	ERROR_MESSAGE = TIMEOUT_ERROR;
	actualFunction = errorState;
}


/**
 * @brief Initializes the UART finite state machine (FSM).
 *
 * - Sets the initial state to @ref idleState.
 * - Initializes the UART peripheral.
 * - Calls @ref Error_Handler if UART initialization fails.
 */
void uartFSM_init() {
	actualFunction = idleState;
	if (!uartInit()) {
		Error_Handler();
	}
}

/**
 * @brief Updates the UART finite state machine (FSM).
 *
 * Executes the function associated with the current state.
 * This should be called periodically in the main loop.
 */
void uartFSM_update() {
	actualFunction();
}
