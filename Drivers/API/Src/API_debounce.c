#include "API_debounce.h"
#include "API_hardware.h"

typedef enum{
 BUTTON_UP,
 BUTTON_FALLING,
 BUTTON_DOWN,
 BUTTON_RAISING,
} debounceState_t;

static debounceState_t lastState;
static void (*actualFunction)(bool_t pin);
static void buttonUp(bool_t pin);
static void buttonDown(bool_t pin);
static bool_t wasPressed;

/**
 * @brief Handles button state transition between UP and DOWN.
 *
 * Updates the FSM state and assigns the corresponding handler function
 * depending on the pin level.
 *
 * @param[in] pin  Current logic level of the button input (true = pressed).
 */
static void buttonTransition(bool_t pin) {
	if (pin) {
		lastState = BUTTON_DOWN;
		wasPressed = true;
		actualFunction = buttonDown;
	} else {
		lastState = BUTTON_UP;
		actualFunction = buttonUp;
	}
}

/**
 * @brief Handles the button UP state.
 *
 * If the button is pressed (pin = true), transitions to the FALLING state
 * and sets the next handler to @ref buttonTransition.
 *
 * @param[in] pin  Current logic level of the button input.
 */
static void buttonUp(bool_t pin) {
	if (pin) {
		actualFunction = buttonTransition;
		lastState = BUTTON_FALLING;
	}
}

/**
 * @brief Handles the button DOWN state.
 *
 * If the button is released (pin = false), transitions to the RAISING state
 * and sets the next handler to @ref buttonTransition.
 *
 * @param[in] pin  Current logic level of the button input.
 */
static void buttonDown(bool_t pin) {
	if (!pin) {
		actualFunction = buttonTransition;
		lastState = BUTTON_RAISING;
	}
}

/**
 * @brief Initializes the button debounce finite state machine (FSM).
 *
 * Sets the initial state to BUTTON_UP and assigns the handler function
 * @ref buttonUp.
 */
void debounceFSM_init() {
	lastState = BUTTON_UP;
	actualFunction = buttonUp;
}
/**
 * @brief Updates the button debounce finite state machine (FSM).
 *
 * Reads the button pin and executes the current state handler function.
 * This function must be called periodically.
 */
void debounceFSM_update(){
	bool_t pressed = !readPin();
	actualFunction(pressed);
}

/**
 * @brief Returns whether the button was pressed since the last call.
 *
 * This function reports a "one-shot" button press event. After being read,
 * the internal flag is cleared until the button is pressed again.
 *
 * @return `true` if the button was pressed since the last call.
 *         `false` otherwise.
 */
bool_t readKey() {
	bool_t key = wasPressed;
	wasPressed = false;
	return key;
}
