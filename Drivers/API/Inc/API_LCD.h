/*
 * API_LCD.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ignaciocl
 */

#ifndef API_INC_API_LCD_H_
#define API_INC_API_LCD_H_
#include <stdbool.h>
#include <stdint.h>
bool WriteString(uint8_t* str);
bool ClearScreen();
bool GoToNextRow();
bool InitLCD();

#endif /* API_INC_API_LCD_H_ */
