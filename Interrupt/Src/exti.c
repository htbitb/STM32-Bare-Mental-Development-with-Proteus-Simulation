/*
 * exti.c
 *
 *  Created on: Apr 20, 2025
 *      Author: hatha
 */


#include "exti.h"

#define GPIOCENR	(1U<<2)
#define SYSCFGENR	(1U<<14)


void pc13_exti_init(void)
{
	/*Disable global interrupt*/
	__disable_irq();

	/*Enable clock access for GPIO*/
	RCC->AHB1ENR |= GPIOCENR;

	/*Configure PC13 as input*/
	GPIOC->MODER &=~((1U<<26));
	GPIOC->MODER &=~((1U<<27));


	/*Enable clock access to SYSCFG*/
	RCC->APB2ENR |= SYSCFGENR;

	/*Select PORTC for EXTI13*/
	SYSCFG->EXTICR[3] |= (1U<<5);

	/*Unmask EXTI13*/
	EXTI->IMR |= (1U<<13);

	/*Select falling edge trigger*/
	EXTI->FTSR |= (1U<<13);

	/*Enable EXTi13 line in NVIC*/
	NVIC_EnableIRQ(40);

	/*Enable global interrupts*/
	__enable_irq();
}
