#include <stm32f4xx.h>
#include "exti.h"

int timestamp = 0;


static void exti_callback(void);

int main()
{
	/**
	 * 1. Enable clock access to GPIOA
	 * */
	RCC->AHB1ENR |= (1U<<0);


	/**
	 * 2. Set PA5 as output pin
	 */
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	pc13_exti_init();


	while(1){
//		/**
//		 * 3. Set Pin5 as a out put high
//		 */
//		while(!(TIM2->SR & SR_UIF));
//		TIM2->SR &= ~SR_UIF;
//		GPIOA->ODR ^= (1U<<5);
//		while(!(TIM3->SR & (1U<<1)));
//
//		/*Read capture value*/
//		timestamp = TIM3->CCR1;
	}
}

static void exti_callback(void)
{
	GPIOA->ODR ^= (1U<<5);
}

void EXTI15_10_IRQHandler(void)
{
	if((EXTI->PR & (1U<<13)) != 0)
	{
		/*Clear PR flag*/
		EXTI->PR &=~(1U<<13);

		/*Do something..*/
		exti_callback();
	}
}
