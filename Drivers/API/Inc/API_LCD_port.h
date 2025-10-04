/*
 * API_LCD_port.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ignaciocl
 */

#ifndef API_INC_API_LCD_PORT_H_
#define API_INC_API_LCD_PORT_H_


#include <stdint.h>
#include <stdbool.h>


bool WriteLCD(uint8_t* mander, bool command);
bool sendInitNibble(uint8_t nibble);



#endif /* API_INC_API_LCD_PORT_H_ */
