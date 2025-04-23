/*
 * i2c.c
 *
 *  Created on: Apr 20, 2025
 *      Author: hatha
 */

#include "i2c.h"

#define SR2_BUSY		(1U<<1)
#define CR1_START		(1U<<8)
#define CR1_STOP		(1U<<9)
#define CR1_ACK			(1U<<10)
#define SR1_SB			(1U<<0)
#define SR1_ADDR		(1U<<1)
#define SR1_TXE			(1U<<7)
#define SR1_RXNE		(1U<<6)
#define SR1_BTF			(1U<<2)

void i2c_init(void)
{
	/*Enable GPIOB clock access*/
	RCC->AHB1ENR |= (1U<<1);

	/*Cofigure GPIOB PB8 as SCL*/
	GPIOB->MODER &= ~(1U<<16);
	GPIOB->MODER |= (1U<<17);

	GPIOB->AFR[1] |= (1U<<1); // alternate function to SCL
	GPIOB->OTYPER |= (1U<<8); //output type open drain

	GPIOB->PUPDR |= (1U<<16); //Pull up

	/*Configure GPIOB PB9 as SDA*/
    GPIOB->MODER &= ~(1U<<18);
	GPIOB->MODER |= (1U<<19);

	GPIOB->AFR[1] |= (1U<<5); //alternate function to SDA
	GPIOB->OTYPER |= (1U<<9); //output type open drain
	GPIOB->PUPDR |= (1U<<18); //Pull up

	/**Enable clock access to I2C1*/
	RCC->APB1ENR |= (1U<<21);

	/*Enable reset mode*/
	I2C1->CR1 |= (1U<<15);

	/*Come out of reset mode*/
	I2C1->CR1 &= ~(1U<<15);

	/*Set peripheral clock frequence*/
	I2C1->CR2 = (1U<<4); // 16 Mhz

	/*I2C to standard mode, 100kHz clock*/
	I2C1->CCR = 80;

	I2C1->TRISE = 17;

	I2C1->CR1 |= (1U<<0); //enable PE

}

void i2c_byteread(char sadrr, char maddr, char* data)
{
	volatile int tmp;

	/*Check busy flag*/
	while(!(I2C1->SR2 & SR2_BUSY));

	/*generate start condition	 */
	I2C1->CR1 |= CR1_START;

	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & SR1_SB));

	/*Transmit slave address + write*/
	I2C1->DR = sadrr << 1;

	/*Wait until addr flag is set*/
	while(!(I2C1->SR1 & SR1_ADDR));

	tmp = I2C1->SR2;

	/*Send memory address*/
	I2C1->DR = maddr;

	/*Wait until transmitter empty*/
	while(!(I2C1->SR1 & SR1_TXE));

	/*Generate restart*/
	I2C1->CR1 |= CR1_START;

	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & SR1_SB));

	/*Transmit slave address + read*/
	I2C1->DR = sadrr << 1 | 1;


	/*Wait until addr flag is set*/
	while(!(I2C1->SR1 & SR1_ADDR));
	tmp = I2C1->SR2;

	I2C1->CR1 |=CR1_STOP;

	/*Wait until RXNE flag is set*/
	while(!(I2C1->SR1 & SR1_RXNE));

	*data++ = I2C1->DR;

}

void i2c_byteWrite(char sadrr, char* data)
{
	volatile int tmp;

	/*Check busy flag*/
	while(!(I2C1->SR2 & SR2_BUSY));

	/*generate start condition	 */
	I2C1->CR1 |= CR1_START;

	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & SR1_SB));

	/*Transmit slave address + write*/
	I2C1->DR = sadrr << 1;

	/*Wait until addr flag is set*/
	while(!(I2C1->SR1 & SR1_ADDR));

	tmp = I2C1->SR2;

	/*Wait until transmitter empty*/
	while(!(I2C1->SR1 & SR1_TXE));

	I2C1->CR1 |=CR1_STOP;

	*data++ = I2C1->DR;

}


void I2C_burstRead(char sadrr, char maddr, char* data, int n)
{
	volatile int tmp;
	/*Check busy flag*/
	while(!(I2C1->SR2 & SR2_BUSY));

	/*generate start condition	 */
	I2C1->CR1 |= CR1_START;

	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & SR1_SB));

	/*Transmit slave address + write*/
	I2C1->DR = sadrr << 1;

	/*Wait until addr flag is set*/
	while(!(I2C1->SR1 & SR1_ADDR));

	tmp = I2C1->SR2;
	(void)tmp;
	/*Send memory address*/
	I2C1->DR = maddr;

	/*Wait until transmitter empty*/
	while(!(I2C1->SR1 & SR1_TXE));

	/*Generate restart*/
	I2C1->CR1 |= CR1_START;

	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & SR1_SB));

	/*Transmit slave address + read*/
	I2C1->DR = sadrr << 1 | 1;


	/*Wait until addr flag is set*/
	while(!(I2C1->SR1 & SR1_ADDR));
	tmp = I2C1->SR2;
	(void)tmp;
	/*Enable Acknowledge*/
	I2C1->CR1 |= CR1_ACK;

	while(n>0)
	{
		/*If one byte*/
		if(n == 1)
		{
			I2C1->CR1 &= ~CR1_ACK; // disable acknowledge

			I2C1->CR1 |=CR1_STOP; // generate stop condition

			/*Wait until RXNE flag is set*/
			while(!(I2C1->SR1 & SR1_RXNE));

			*data++ = I2C1->DR;
			break;
		}
		else
		{
			/*Wait until RXNE flag is set*/
			while(!(I2C1->SR1 & SR1_RXNE));

			*data++ = I2C1->DR;

			n--;
		}
	}
}


void I2C_burstWrite(char sadrr, char maddr, char* data, int n)
{
	volatile int tmp;

	/*Check busy flag*/
	while(!(I2C1->SR2 & SR2_BUSY));

	/*generate start condition	 */
	I2C1->CR1 |= CR1_START;

	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & SR1_SB));

	/*Transmit slave address + write*/
	I2C1->DR = sadrr << 1;

	/*Wait until addr flag is set*/
	while(!(I2C1->SR1 & SR1_ADDR));

	tmp = I2C1->SR2;
	(void)tmp;
	/*Wait until transmitter empty*/
	while(!(I2C1->SR1 & SR1_TXE));

	/*Send memory address*/
	I2C1->DR = maddr;

	for (int i =0; i < n; i++)
	{
		while(!(I2C1->SR1 & SR1_TXE));

		I2C1->DR = *data++;
	}

	while(!(I2C1->SR1 & SR1_BTF));

	I2C1->CR1 |=CR1_STOP; // generate stop condition

}
