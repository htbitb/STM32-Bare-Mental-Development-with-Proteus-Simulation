#include <stm32f4xx.h>

#define ADC1EN	(1U<<8)
#define ADC_CH1 (1U<<0)
#define CR1_EOCIE	(1U<<5)

void start_conversion(void);
void PA1_ADC_init(void);
uint32_t adc_read(void);
void PA1_ADC_interrupt_init(void);
void adc_callback(void);

uint32_t sensor_value;
uint32_t temperature;
int main()
{
	uint32_t *temp;
	temp = (uint32_t*)0x4001203B;
	/**
	 * 1. Enable clock access to GPIOA
	 * */
	RCC->AHB1ENR |= (1U<<0);

	/**
	 * 2. Set PA5 as output pin
	 */
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	PA1_ADC_interrupt_init();


	while(1)
	{
		start_conversion();
		sensor_value = (adc_read() * 5.0) / 1024.0;
//		temperature	= (sensor_value - 0.1) * 100.0;
		*temp = adc_read();
		if(temperature > 24)
		{
			GPIOA->ODR |= (1U<<5);
		}
		else
		{
			GPIOA->ODR &= ~(1U<<5);
		}
	}

	return 0;
}
void adc_callback(void)
{

}

void ADC_IRRHandler(void)
{
	/*Check for eoc in SR*/
	if((ADC1->SR & CR1_EOCIE) != 0)
	{
		/*Clear EOC*/
		ADC1->SR &= ~CR1_EOCIE;

		/*Do something*/
		adc_callback();
	}
}

void PA1_ADC_interrupt_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= (1U<<0);

	/*Set the mode of PA1 to analog*/
	GPIOA->MODER |= (1U<<2);
	GPIOA->MODER |= (1U<<3);


	/************** Configure the ADC module *******************/

	/*Enable clock access to adc pin's port*/
	RCC->APB2ENR |= ADC1EN;

	/*Enable ADC end-of-conversion interrupt*/
	ADC1->CR1 |= CR1_EOCIE;

	/*Enable ADC interrupt in NVIC*/
	NVIC_EnableIRQ(ADC_IRQn)

	/****************** Configure adc parameter****************/
	/*Conversion sequence start*/
	ADC1->SQR3 |= ADC_CH1;

	/*Conversion sequence length*/
	ADC1->SQR1 = 0x0U;

	/*Enable ADC module*/
	ADC1->CR2 |= (1U<<0);


}

void PA1_ADC_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= (1U<<0);

	/*Set the mode of PA1 to analog*/
	GPIOA->MODER |= (1U<<2);
	GPIOA->MODER |= (1U<<3);


	/************** Configure the ADC module *******************/

	/*Enable clock access to adc pin's port*/
	RCC->APB2ENR |= ADC1EN;

	/****************** Configure adc parameter****************/
	/*Conversion sequence start*/
	ADC1->SQR3 |= ADC_CH1;

	/*Conversion sequence length*/
	ADC1->SQR1 = 0x0U;

	/*Enable ADC module*/
	ADC1->CR2 |= (1U<<0);


}

void start_conversion(void)
{
	/*Start adc conversion*/
	ADC1->CR2 |= (1U<<30);


}

uint32_t adc_read(void)
{
	/*Wait for conversion to be complete*/
	while(!(ADC1->SR & (1U<<1)));

	/*Read converted result*/
	return (ADC1->DR);
}
