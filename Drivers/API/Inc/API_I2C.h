/*
 * API_I2C.h
 *
 *  Created on: Oct 1, 2025
 *      Author: ignaciocl
 */

#ifndef API_INC_API_I2C_H_
#define API_INC_API_I2C_H_


#include <stdbool.h>
#include <stdint.h>

bool Transmit(uint16_t unshiftedAddress, uint8_t* value);
bool TransmitToSize(uint16_t unshiftedAddress, uint8_t* value, uint16_t size);
bool ReceiveToSize(uint16_t unshiftedAddress, uint8_t* value, uint16_t size);

#endif /* API_INC_API_I2C_H_ */
