#include <stm32f4xx.h>
#include "i2c.h"

int main()
{

	i2c_init();
	char data = "A";

	while(1)
	{
		i2c_byteWrite(0x60, &data);
	}

	return 0;
}

