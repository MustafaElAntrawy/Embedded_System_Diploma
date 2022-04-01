/*
 * I2C_Slave_EEROM.h
 *
 *  Created on: Apr 1, 2022
 *      Author: moustafa
 */

#ifndef INCLUDES_I2C_SLAVE_EEROM_H_
#define INCLUDES_I2C_SLAVE_EEROM_H_

#include "Stm32F103C6_I2C_Driver.h"

#define EEPROM_Slave_Address		0x2A

void eeprom_init(void);
unsigned char eeprom_write_Nbytes(unsigned int Memory_address, unsigned char* bytes, uint8_t Data_Length);
unsigned char eeprom_read_byte(unsigned int address, unsigned char* dataOut, uint8_t dataLen);

#endif /* INCLUDES_I2C_SLAVE_EEROM_H_ */
