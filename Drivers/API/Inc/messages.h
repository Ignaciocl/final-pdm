/*
 * messages.h
 *
 *  Created on: Oct 4, 2025
 *      Author: ignaciocl
 */

#ifndef API_INC_MESSAGES_H_
#define API_INC_MESSAGES_H_
// Information messages
uint8_t* ROW_ONE_WELCOME = (uint8_t*)"press button";
uint8_t* ROW_TWO_WELCOME = (uint8_t*)"to start";
uint8_t* UART_WELCOME = (uint8_t*)"\n\rPress button to start";
uint8_t* READ_DISTANCE = (uint8_t*)"Distance read:";
uint8_t* DISTANCE_UNIT = (uint8_t*)" centimeters";
uint8_t* UART_READ_DISTANCE = (uint8_t*)"\n\rDistance read:";
uint8_t* CALCULATING_DISTANCE = (uint8_t*)"CALCULATING";
uint8_t* UART_CALCULATING_DISTANCE = (uint8_t*)"\n\rCalculating distance with sensor";

// Error messages
uint8_t* ERROR_INIT_UART = (uint8_t*) "\n\rThere was an error on the initialization";

#endif /* API_INC_MESSAGES_H_ */
