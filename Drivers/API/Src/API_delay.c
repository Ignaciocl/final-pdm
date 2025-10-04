#include "API_delay.h"
#include "API_hardware.h"
#include <stddef.h>

void Error_Handler(void); // Implemented in main.c always and defined in its main.h

static void errorIfNeeded(delay_t* delay) {
	if (delay == NULL) {
		Error_Handler();
	}
}

/**
 * @brief Reads the state of a non-blocking delay.
 *
 * - If the delay was not running, it starts it and returns `false`.
 * - If it was already running, it checks whether the configured time has elapsed.
 *
 * @param[in,out] delay  Pointer to the delay structure.
 *
 * @return `true` if the delay has expired.
 *         `false` if the delay is still running.
 */
bool_t delayRead( delay_t * delay ) {
	errorIfNeeded(delay);
	if (!delay->running) {
		delay->running = true;
		delay->startTime = getTick();
		return false;
	}
	delay->running = (getTick() - delay->startTime) < delay->duration;
	return !delay->running;
}

/**
 * @brief Updates the duration of a non-blocking delay.
 *
 * The duration is only updated if the delay is not currently running.
 *
 * @param[in,out] delay     Pointer to the delay structure.
 * @param[in]     duration  New duration in milliseconds.
 */
void delayWrite( delay_t * delay, tick_t duration ){
	errorIfNeeded(delay);
	delay->duration = duration;
}

/**
 * @brief Initializes a non-blocking delay structure.
 *
 * Sets the delay duration in milliseconds and initializes the running state
 * as false (`running = false`).
 *
 * @param[in,out] delay     Pointer to the delay structure to initialize.
 * @param[in]     duration  Delay duration in milliseconds.
 */
void delayInit( delay_t * delay, tick_t duration ) {
	errorIfNeeded(delay);
	delay->duration = duration;
	delay->running = false;
}

/**
 * @brief Checks whether a delay is currently running.
 *
 * @param[in] delay  Pointer to the delay structure.
 *
 * @return `true` if the delay is running.
 *         `false` otherwise.
 */
bool_t delayIsRunning(delay_t * delay) {
	errorIfNeeded(delay);
	return delay->running;
}
