#include <stdint.h>
#include <stdio.h>

#define GPIO_BASE	(0x40020000UL)
#define RCC_BASE	(0x40023800UL)
#define __IO volatile

// Create a structure for RCC registers
typedef struct{
	__IO uint32_t RCC_CR;
	__IO uint32_t RCC_DUMMY[11];
	__IO uint32_t RCC_AHB1ENR;
	__IO uint32_t RCC_APB1ENR;
	__IO uint32_t RCC_APB2ENR;
}RCC_typedef, *RCC_ptr;

// Create a structure for GPIO registers
typedef struct{
	__IO uint32_t GPIOx_MODER;
	__IO uint32_t GPIOx_DUMMY[3];
	__IO uint32_t GPIOx_IDR;
	__IO uint32_t GPIOx_ODR;
	__IO uint32_t GPIOx_BSRR;
	__IO uint32_t GPIOx_LCKR;
}GPIO_typedef, *GPIO_ptr;

#define RCC		((RCC_typedef*)RCC_BASE)
#define GPIOA	((GPIO_typedef*)GPIO_BASE)


int main()
{
	/**
	 * 1. Enable clock access to GPIOA
	 * */
	RCC->RCC_AHB1ENR |= (1U<<0);


	/**
	 * 2. Set PA5 as output pin
	 */
	GPIOA->GPIOx_MODER |= (1U<<10);
	GPIOA->GPIOx_MODER &= ~(1U<<11);

	while(1){
		/**
		 * 3. Set Pin5 as a out put high
		 */
		GPIOA->GPIOx_ODR |= (1U<<5);

	}
}
