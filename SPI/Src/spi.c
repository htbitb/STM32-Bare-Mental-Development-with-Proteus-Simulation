/*
 * spi.c
 *
 *  Created on: Apr 21, 2025
 *      Author: hatha
 */


#include "spi.h"
#include <stdint.h>

#define SPI1EN		(1U<<12)
#define GPIOAEN		(1U<<0)

#define SR_TXE		(1U<<1)
#define SR_BSY		(1U<<7)
#define SR_RXNE		(1U<<0)

/* PA5 ->CLK
 * PA6 -> MISO
 * PA7 -> MOSI
 * PA9 -> Slave select
 */

void spi_gpio_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*********** Set gpio alternate function ****************/
	/*PA5*/
	GPIOA->MODER &= ~(1U<<10);
	GPIOA->MODER |= (1U<<11);

	/*PA6*/
	GPIOA->MODER &= ~(1U<<12);
	GPIOA->MODER |= (1U<<13);

	/*PA7*/
	GPIOA->MODER &= ~(1U<<14);
	GPIOA->MODER |= (1U<<15);

	/*PA9*/
	GPIOA->MODER |= (1U<<18);
	GPIOA->MODER &= ~(1U<<19);

	/*Set gpio function type SPI*/
	/*CLK*/
	GPIOB->AFR[0] |= (5<<20);

	/*MISO*/
	GPIOB->AFR[0] |= (5<<24);

	/*MOSI*/
	GPIOB->AFR[0] |= (5<<28);

}

void spi1_config(void)
{
	 /*Enalbe clock access to SPI1 module*/
	RCC->APB2ENR |= SPI1EN;

	/*Set clock to fPCLK/4*/
	SPI1->CR1 |= (1U<<3);
	SPI1->CR1 &=~(1U<<4);
	SPI1->CR1 &=~(1U<<5);

	/*Enable full duplex*/
	SPI1->CR1 &=~(1U<<10);

	/*Set MSB first*/
	SPI1->CR1 &=~(1U<<7);

	/*Set master mode*/
	SPI1->CR1 |= (1U<<2);

	/*Set 8 bit data mode*/
	SPI1->CR1 &= ~(1U<<11);

	/*Select SW salve management by
	 * setting SSM=1 and SSi=1*/
	SPI1->CR1 |=(1U<<8);
	SPI1->CR1 |= (1U<<9);


	/*Enable SPI communicate*/
	SPI1->CR1 |= (1<<6);

}

void spi1_transmit(uint8_t *data, uint32_t size)
{
	uint32_t i = 0;
	uint8_t tmp;

	while(i < size)
	{
		/*Wait until TXE is set*/
		while(!(SPI1->SR & SR_TXE));

		/*write the data to the data register*/
		SPI1->DR = data[i];
		i++;

	}

	/*Wait until TXE is set*/
	while(!(SPI1->SR & SR_TXE));

	/*Wait for BUSY flag to reset*/
	while((SPI1->SR & SR_BSY));

	/*Clear OVR flag*/
	tmp = SPI1->DR;
	tmp = SPI1->SR;
}

void spi1_receive(uint8_t *data, uint32_t size)
{
	while(size)
	{
		/*send dummy data*/
		SPI1->DR = 0;

		/*Wait for RXNE is set*/
		while(!(SPI1->SR & SR_RXNE));

		/*Read data from data register*/
		*data++ = (SPI1->DR);
		size--;
	}
}

void cs_enable(void)
{
	GPIOA->ODR &= ~(1U<<9);
}

void cs_disable(void)
{
	GPIOA->ODR |= (1U<<9);
}
