#include <stm32f4xx.h>


#define SYSTICK_LOAD_VAL		16000000
#define CTRL_ENABLE				(1U<<0)
#define CTRL_CLKSRC				(1U<<2)
#define CTRL_COUNTFLAG			(1U<<16)
#define ONE_SEC_LOAD			16000000
#define CTRL_TICKINT			(1U<<1)

void systickDemayMS(int delay);
void systick_1hz_interrupt(void);

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

	systick_1hz_interrupt();

	while(1){
//		/**
//		 * 3. Set Pin5 as a out put high
//		 */
//		GPIOA->ODR ^= (1U<<5);
//
//		systickDemayMS(10000);

	}
}


void systickDemayMS(int delay)
{
	/************** configure systick*****************/

	SysTick->LOAD = SYSTICK_LOAD_VAL;

	/*clear systick current value*/
	SysTick->VAL = 0;

	/*Enable systick and select internal clk src*/
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	for(int i=0; i < delay; i++)
	{
		/*wait until the countflag is set*/
		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0);

	}
	SysTick->CTRL = 0;
}

void systick_1hz_interrupt(void)
{
	/*Reload with number of clocks persecond*/
	SysTick->LOAD = ONE_SEC_LOAD;

	/*clear systick current value*/
	SysTick->VAL = 0;

	/*Enable systick and select internal clk src*/
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	/*Enable Systick interrupt*/
	SysTick->CTRL |= CTRL_TICKINT;

	/*Clear */

}

void SysTick_Handler(void)
{
	GPIOA->ODR ^= (1U<<5);
}
