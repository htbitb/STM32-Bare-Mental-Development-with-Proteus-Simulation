/*
 * stm32f40xx_rcc_driver.h
 *
 *  Created on: Apr 10, 2025
 *      Author: hatha
 */

#ifndef STM32F40XX_RCC_DRIVER_H_
#define STM32F40XX_RCC_DRIVER_H_

#include "stm32f40xx.h"

//This returns the APB1 clock value
U32 RCC_GetPCLK1Value(void);

//This returns the APB2 clock value
U32 RCC_GetPCLK2Value(void);


U32  RCC_GetPLLOutputClock(void);

#endif /* STM32F40XX_RCC_DRIVER_H_ */
