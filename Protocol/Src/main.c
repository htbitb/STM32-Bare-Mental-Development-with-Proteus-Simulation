#include <stm32f4xx.h>

#define SYS_CLK 16000000
#define USART_BAUDRATE 9600

void usart_init(USART_TypeDef *USARTx);
uint32_t usart_baudrate_cal(uint32_t periphclk, uint32_t baudrate);
void baudrate_init(USART_TypeDef *USARTx, uint32_t periphclk, uint32_t baudrate);
void usart_tx(USART_TypeDef *USARTx, int ch);

int main()
{
	RCC->AHB1ENR |= (1U<<0);
//	RCC->CR |= (1U << 16);

	GPIOA->MODER |= (1U<<5);
	GPIOA->MODER &=~(1U<<4);


	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<11);

	RCC->APB1ENR |= (1U << 17);

	usart_init(USART2);

	while(1)
	{
		usart_tx(USART2, "A");
		usart_tx(USART2, "B");
		usart_tx(USART2, "C");
	}

	return 0;
}

void usart_tx(USART_TypeDef *USARTx, int ch)
{
	while(!(USARTx->SR & (1U<<7)));

	USARTx->DR = (ch & 0xFF);
}

void usart_init(USART_TypeDef *USARTx)
{
	USARTx->CR1 |= (1U<<3);

	USARTx->CR1 |= (1U<<13);

//	baudrate_init(USARTx, SYS_CLK, USART_BAUDRATE);


	USARTx->BRR = 0x0EA6;

}
//
//void baudrate_init(USART_TypeDef *USARTx, uint32_t periphclk, uint32_t baudrate)
//{
//	USARTx->BRR = usart_baudrate_cal(periphclk, baudrate);
//}
//
//uint32_t usart_baudrate_cal(uint32_t periphclk, uint32_t baudrate)
//{
//	return ((periphclk + (baudrate / 2U)) / baudrate);
//}
