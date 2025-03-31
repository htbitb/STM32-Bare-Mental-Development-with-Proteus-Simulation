#include <stm32f40xx.h>
#include <string.h>

int main()
{
	/**
//	 * GPIO clock peripheral enable
//	 * */
//	GPIO_PeriClockControl(GPIO_A, ENABLE);
	GPIO_PeriClockControl(GPIO_B, ENABLE);
//
//	/** AF05
//	 * NSS: PA15
//	 * SCK: PB3
//	 * MISO: PB4
//	 * MOSI: PB5
//	 */
	GPIO_Handle pGPIOB;


//
//	 pGPIOA.pGPIOx = GPIOA;
//	 pGPIOA.GPIO_PmapConfig.GPIO_PinNumber = GPIO_PIN3;
//	 pGPIOA.GPIO_PmapConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
//	 pGPIOA.GPIO_PmapConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
////	 pGPIOA.GPIO_PmapConfig.GPIO_PinAltFunMode = 5;
//
//
//	 GPIO_init(GPIO_A, &pGPIOA);

	pGPIOB.pGPIOx = GPIOB;
	pGPIOB.GPIO_PmapConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	pGPIOB.GPIO_PmapConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	pGPIOB.GPIO_PmapConfig.GPIO_PinAltFunMode = 5;


	pGPIOB.GPIO_PmapConfig.GPIO_PinNumber = GPIO_PIN15;
	GPIO_init(GPIO_B, &pGPIOB);

	pGPIOB.GPIO_PmapConfig.GPIO_PinNumber = GPIO_PIN13;
	GPIO_init(GPIO_B, &pGPIOB);



	/**
	 * SPI clock
	 */
	SPIx_PeriClockControl(SPI_2, ENABLE);
	SPI_PeripheralControl(SPI2, ENABLE);

	/**
	 * SPI inits
	 */

	SPI_Handle_tp SPI1handle;

	SPI1handle.pSPIx = SPI2;
	SPI1handle.SPIConf.SPI_BusConfig = SPI_BUS_CFG_FD;
	SPI1handle.SPIConf.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI1handle.SPIConf.SPI_SCLKSpeed = SPI_SCLK_SPEED_DIV2;
	SPI1handle.SPIConf.SPI_DFF = SPI_DFF_8BITS;
	SPI1handle.SPIConf.SPI_CPOL = SPI_CPOL_LOW;
	SPI1handle.SPIConf.SPI_CPHA = SPI_CPHA_LOW;
	SPI1handle.SPIConf.SPI_SSM = 0;

	SPIx_init(SPI_2, &SPI1handle);
	SPI_SSIConfig(SPI2, ENABLE);

	char user_data[] = "Hello World";

	SPI1->DR = 10;

	SPI_DataTransfer(SPI1, (U08*)user_data, strlen(user_data));

	while(1);


}


