/*
 * i2c.h
 *
 *  Created on: Apr 20, 2025
 *      Author: hatha
 */

#ifndef I2C_H_
#define I2C_H_

#include <stm32f4xx.h>

void i2c_init(void);
void i2c_read(char sadrr, char maddr, char* data);
void i2c_byteWrite(char sadrr, char* data);
void I2C_burstRead(char sadrr, char maddr, char* data, int n);
void I2C_burstWrite(char sadrr, char maddr, char* data, int n);

#endif /* I2C_H_ */
