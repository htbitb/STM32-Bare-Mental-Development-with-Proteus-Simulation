#include <stm32f40xx_SPI_driver.h>



U08 SPI_GetFlagStatus(SPI_TypeDef* pSPIx, U32 FlagName)
{
    if(pSPIx->SR & FlagName)
    {
        return FLAG_SET;
    }
    return FLAG_RESET;
}



 /**
  * Peripheral clock setup
  */
void SPIx_PeriClockControl(U08 SPIx, U08 EnorDis)
{
    if (EnorDis == ENABLE)
    {
        SPIx_PCLOCK_EN(SPIx);
    }
    else
    {
        // do nothing
    }
}

/**
 * Init and De-init
 */
void SPIx_init(U08 SPIx, SPI_Handle_tp* pSPIHandle)
{
    U32 tempreg = 0;
    /* First configure the SPI_CR1 register*/
    /* Configure the device mode*/
    tempreg |= pSPIHandle->SPIConf.SPI_DeviceMode << CR1_MSTR;

    /* Configure the bus cfg*/
    if (pSPIHandle->SPIConf.SPI_BusConfig == SPI_BUS_CFG_FD)
    {
        tempreg &= ~(1 << CR1_BIDI_MODE);
    }
    else if (pSPIHandle->SPIConf.SPI_BusConfig == SPI_BUS_CFG_HD)
    {
        tempreg |= (1 << CR1_BIDI_MODE);
    }

    else if (pSPIHandle->SPIConf.SPI_BusConfig == SPI_BUS_CFG_SP_RXONLY)
    {
        tempreg &= ~(1 << CR1_BIDI_MODE);
        tempreg |= (1 << CR1_RX_ONLY);
    }

    // 3. Configure the spi serial clock speed (baud rate)
	tempreg |= pSPIHandle->SPIConf.SPI_SCLKSpeed << CR1_BR;

	//4.  Configure the DFF
	tempreg |= pSPIHandle->SPIConf.SPI_DFF << CR1_DFF;

	//5. configure the CPOL
	tempreg |= pSPIHandle->SPIConf.SPI_CPOL << CR1_CPOL;

	//6 . configure the CPHA
	tempreg |= pSPIHandle->SPIConf.SPI_CPHA << CR1_CPHA;

	tempreg |= pSPIHandle->SPIConf.SPI_SSM << CR1_SSM;

	pSPIHandle->pSPIx->CR1 = tempreg;
}

void SPIx_DeInit(U08 SPIx)
{
    
}

/**
 * Data Send and Receive
 */
void SPI_DataTransfer(SPI_TypeDef* pSPIx, U08 *pTxBuffer, U32 len)
{
    while(len > 0)
    {
        //1. wait until TXE is set
        while (SPI_GetFlagStatus(pSPIx, SPI_FLAG_TXE) == RESET);

        //2. Check the DFF bit in the DR
        if (pSPIx->CR1 & (1U << CR1_DFF))
        {
            //16 bits
            pSPIx->DR = *((U16*)pTxBuffer);
            len--;
			len--;
			(U16*)pTxBuffer++;
        }
        else
        {
            pSPIx->DR = *(pTxBuffer);
            len--;
            pTxBuffer++;
        }
    }
}

void SPI_PeripheralControl(SPI_TypeDef* pSPIx, U08 EnorDis)
{
	if(EnorDis == ENABLE)
	{
		pSPIx->CR1 |= (1 << CR1_SPE);
	}
    else
    {
        pSPIx->CR1 &= ~(1 << CR1_SPE);
    }
}

void  SPI_SSIConfig(SPI_TypeDef *pSPIx, U08 EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |=  (1 << CR1_SSI);
	}else
	{
		pSPIx->CR1 &=  ~(1 << CR1_SSI);
	}


}

void SPI_DataReceive(SPI_TypeDef* pSPIx, U08 *pRxBuffer, U32 len)
{

}

/**
 * IRQ Configuration and ISR handling
 */
void SPI_IRQIRConfig(U08 IRQNumber, U08 EnorDis)
{

}

void SPI_IRQPriorityConfig(U08 IRQNumber, U08 IRQPriority)
{

}

void SPI_IRQHandling(SPI_Handle_tp* pSPIHandle)
{

}
