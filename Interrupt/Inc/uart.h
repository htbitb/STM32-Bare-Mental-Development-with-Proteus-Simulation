/*
 * uart.h
 *
 *  Created on: Apr 20, 2025
 *      Author: hatha
 */

#ifndef UART_H_
#define UART_H_

#include <stm32f4xx.h>

#define SYS_CLK 16000000
#define USART_BAUDRATE 9600
#define CR1_RXNEIE		(1U<<5)

void usart_init(USART_TypeDef *USARTx);
uint32_t usart_baudrate_cal(uint32_t periphclk, uint32_t baudrate);
void baudrate_init(USART_TypeDef *USARTx, uint32_t periphclk, uint32_t baudrate);
void usart_tx(USART_TypeDef *USARTx, int ch);

#endif /* UART_H_ */
