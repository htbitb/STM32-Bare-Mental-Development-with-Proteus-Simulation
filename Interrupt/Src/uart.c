/*
 * uart.c
 *
 *  Created on: Apr 20, 2025
 *      Author: hatha
 */
#include "uart.h"

void usart_tx(USART_TypeDef *USARTx, int ch)
{
	while(!(USARTx->SR & (1U<<7)));

	USARTx->DR = (ch & 0xFF);
}

void usart_init(USART_TypeDef *USARTx)
{
	USARTx->CR1 |= (1U<<3);

	USARTx->CR1 |= (1U<<13);

	baudrate_init(USARTx, SYS_CLK, USART_BAUDRATE);


}

void baudrate_init(USART_TypeDef *USARTx, uint32_t periphclk, uint32_t baudrate)
{
	USARTx->BRR = usart_baudrate_cal(periphclk, baudrate);
}

uint32_t usart_baudrate_cal(uint32_t periphclk, uint32_t baudrate)
{
	return ((periphclk + (baudrate / 2U)) / baudrate);
}

void usart_rx_interrupt(USART_TypeDef *USARTx)
{
	USARTx->CR1 |= (1U<<3);

	USARTx->CR1 |= (1U<<13);

	baudrate_init(USARTx, SYS_CLK, USART_BAUDRATE);

	/**Configure the transfer direction	 */
	USARTx->CR1
	/*Enable uart module*/
	USARTx->CR1 |= CR1_RXNEIE;



}
