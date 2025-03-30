#include <stm32f40xx.h>
#include <string.h>

int main()
{
	/**
	 * GPIO clock peripheral enable
	 * */
	// GPIO_PeriClockControl(GPIO_A, ENABLE);
	GPIO_PeriClockControl(GPIO_B, ENABLE);


	/**
	 * GPIOA configuration as Push pull 
	 */
	GPIO_Handle GpioLed;
	memset(&GpioLed, 0, sizeof(GpioLed));

	GpioLed.pGPIOx = GPIOB;
	GpioLed.GPIO_PmapConfig.GPIO_PinNumber = GPIO_PIN12;
	GpioLed.GPIO_PmapConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	GpioLed.GPIO_PmapConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PmapConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_init(GPIO_A, &GpioLed);

	/**
	 * Button configuration
	 */
	GPIO_Handle GpioBtn;
	memset(&GpioBtn, 0, sizeof(GpioBtn));

	GpioBtn.pGPIOx = GPIOB;
	GpioBtn.GPIO_PmapConfig.GPIO_PinNumber = GPIO_PIN5;
	GpioBtn.GPIO_PmapConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GpioBtn.GPIO_PmapConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_init(GPIO_B, &GpioBtn);

	/**
	 * IRQ configuration
	 */
	GPIO_IRQIRConfig(IRQ_NO_EXTI9_5, ENABLE);
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5, 15);
	while(1);

}

void EXTI9_5_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN5);
	GPIO_ToggleOutputPin(GPIOB, GPIO_PIN12);
}

