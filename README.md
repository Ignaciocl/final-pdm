# Práctica 5 - UART and Command Parser FSM

## Objective
Implement a software module to use the UART and a Finite State Machine (FSM) to parse commands received via UART in polling mode (without interrupts or DMA), using the STM32 HAL (STM32F4 + STM32CubeIDE, C).

---

## Point 1 - UART Module

Create a software module with the following files:
- `API_uart.c` (inside `/API/src`)
- `API_uart.h` (inside `/API/inc`)

### Public functions (in `API_uart.h`):
```c
bool_t uartInit();
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);
```

### Implementation details (in `API_uart.c`):
- `uartInit()` initializes the UART and additionally prints a configuration message over the serial terminal.  
  - Returns:
    - `true`: initialization successful
    - `false`: initialization failed
- `uartSendString(uint8_t *pstring)`: transmits a null-terminated string using `HAL_UART_Transmit(...)`.
- `uartSendStringSize(uint8_t * pstring, uint16_t size)`: transmits a string with a fixed size using `HAL_UART_Transmit(...)`.
- `uartReceiveStringSize(uint8_t * pstring, uint16_t size)`: receives a string of fixed size using `HAL_UART_Receive(...)`.
- All functions must validate:
  - Pointer parameters are not `NULL`.
  - Size parameters are within reasonable limits.
- The return values of all HAL UART functions used must be verified.

---

## Point 2 (Optional) - Command Parser FSM

Design and implement a module:
- `API_cmdparser.c` (inside `/API/src`)
- `API_cmdparser.h` (inside `/API/inc`)

### Definitions
```c
#define CMD_MAX_LINE 64       // bytes, includes '\0' terminator
#define CMD_MAX_TOKENS 5      // COMMAND + up to 4 args

typedef enum {
  CMD_OK = 0,
  CMD_ERR_TIMEOUT,
  CMD_ERR_OVERFLOW,
  CMD_ERR_SYNTAX,
  CMD_ERR_UNKNOWN,
  CMD_ERR_ARG
} cmd_status_t;
```

### FSM States
- **CMD_IDLE**: waits for characters, transitions to CMD_RECV on any non-terminator character.
- **CMD_RECV**: accumulates characters until `\r` or `\n`.
  - If buffer exceeds `CMD_MAX_LINE - 1` → `CMD_ERROR (OVERFLOW)`.
  - On terminator → `CMD_PARSE`.
- **CMD_PARSE**: tokenizes input, normalizes command, validates syntax/arguments.
  - Valid → `CMD_EXEC`.
  - Invalid → `CMD_ERROR (SYNTAX | ARG | UNKNOWN)`.
- **CMD_EXEC**: executes command handler, prints response, returns to `CMD_IDLE`.
- **CMD_ERROR**: prints error, clears buffer, returns to `CMD_IDLE`.
- **CMD_TIMEOUT (optional)**: aborts if no terminator within timeout, returns to `CMD_IDLE`.

### Protocol Rules
- Line format: `COMMAND [arg1 [arg2...]]` terminated with `\r\n`, `\n`, or `\r`.
- Case-insensitive commands, tolerates multiple spaces.
- Lines starting with `#` or `//` are ignored.
- Responses end with `\r\n`.
- Minimum set of recognized commands:
  - `HELP`
  - `LED ON|OFF|TOGGLE`
  - `STATUS`
  - `BAUD?`
  - `BAUD=<num>` (9600..921600)
  - `CLEAR`
- Error messages:
  - `ERROR: unknown command`
  - `ERROR: bad args`
  - `ERROR: line too long`
  - `ERROR: timeout`

---

## Summary
This practice implements:
1. A UART driver (`API_uart`) with safe parameter checking and HAL integration.
2. (Optional) A command parser FSM (`API_cmdparser`) that processes UART input, parses commands, validates them, and executes system actions.

Tested on **STM32F4 NUCLEO board** with STM32CubeIDE.
