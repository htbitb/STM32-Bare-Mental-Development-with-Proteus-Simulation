#include <stm32f4xx.h>


#define TIM2ENR		(1U<<0)
#define TIM3ENR		(1U<<1)
#define CR1_CEN		(1U<<0)
#define SR_UIF		(1U<<0)
#define CCER_CC1S	(1U<<0)

void tim2_1hz_init(void);
void tim2_output_compare(void);
void tim3_input_capture(void);
void tim2_1hz_interrupt(void);

int timestamp = 0;
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

	tim2_1hz_interrupt();
//	GPIOA->AFR[0] |= (1U<<20);

//	tim2_output_compare();
//	tim3_input_capture();

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

void tim2_1hz_init(void)
{
	/*Enable Clock access*/
	RCC->APB1ENR |= TIM2ENR;

	/*Set pre-scaler value*/
	TIM2->PSC = 1600 - 1;

	/*Set auto-reload value*/
	TIM2->ARR = 10000 - 1;

	/*Clear counter*/
	TIM2->CNT = 0;

	/*Enable timer*/
	TIM2->CR1 |= CR1_CEN;
}

void tim2_1hz_interrupt(void)
{
	/*Enable Clock access*/
	RCC->APB1ENR |= TIM2ENR;

	/*Set pre-scaler value*/
	TIM2->PSC = 1600 - 1;

	/*Set auto-reload value*/
	TIM2->ARR = 10000 - 1;

	/*Clear counter*/
	TIM2->CNT = 0;

	/*Enable timer*/
	TIM2->CR1 |= CR1_CEN;

	/*Enable TIM interrupt*/
	TIM2->DIER |= (1U<<0);

	/**Enable TIM interrupt in NVIC*/
	NVIC_EnableIRQ(TIM2_IRQn);
}

void tim2_output_compare(void)
{
	/*Enable Clock access*/
	RCC->APB1ENR |= TIM2ENR;

	/*Set pre-scaler value*/
	TIM2->PSC = 1600 - 1;

	/*Set auto-reload value*/
	TIM2->ARR = 10000 - 1;

	/*Set output compare toggle mode*/
	TIM2->CCMR1 = (1U<<4) | (1U<<5);

	/*Enable tim2 ch1 in compare mode*/
	TIM2->CCER |= (1U<<0);

	/*Clear counter*/
	TIM2->CNT = 0;

	/*Enable timer*/
	TIM2->CR1 |= CR1_CEN;
}


void tim3_input_capture(void)
{
	/************* GPIO Configure***********************/
	/*Enable clock access to GPIOB*/
	RCC->AHB1ENR |= (1U<<1);

	/*PB4 alternate function configure*/
	GPIOB->MODER &= ~(1U<<8);
	GPIOB->MODER |= (1U<<9);

	/*AF02 configure*/
	GPIOB->AFR[0] |= (1U<<17);

	/************* Input capture mode configure*************/
	/*Enable Clock access*/
	RCC->APB1ENR |= TIM3ENR;

	/*Set pre-scaler value*/
	TIM3->PSC = 1600 - 1;

	/*Set auto-reload value*/
	TIM3->ARR = 10000 - 1;

	/*Set CH1 to input capture*/
	TIM3->CCMR1 |= CCER_CC1S;

	/**/
	TIM3->CCER |= (1U<<0);

	/*Enable timer*/
	TIM3->CR1 |= CR1_CEN;
}

void TIM2_IRQHandler(void)
{
	/*Clear update interrupt flag*/
	TIM2->SR &= ~(1U<<0);

	GPIOA->ODR ^= (1U<<5);

}
