#include <stm32f4xx.h>
#include "spi.h"


int main()
{
	int data = "A";
	spi_gpio_init();
	spi1_config();

	while(1)
	{
		cs_enable();
		spi1_transmit(&data, 1);
		cs_disable();
	}

	return 0;
}

