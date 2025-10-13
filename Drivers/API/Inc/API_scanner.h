/*
 * API_scanner.h
 *
 *  Created on: Oct 3, 2025
 *      Author: ignaciocl
 */

#ifndef API_INC_API_SCANNER_H_
#define API_INC_API_SCANNER_H_

#include <stdbool.h>
#include <stdint.h>

bool ReadSensor(uint16_t* valueRead);
bool WriteSensor();


#endif /* API_INC_API_SCANNER_H_ */
