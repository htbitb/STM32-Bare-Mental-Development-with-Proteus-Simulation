#include <stm32f40xx_SPI_driver.h>

PRIVATE void spi_txe_interrupt_handler(SPI_Handle_tp* pSPIHandle);
PRIVATE void spi_rxe_interrupt_handler(SPI_Handle_tp* pSPIHandle);
PRIVATE void spi_ovr_err_interrupt_handler(SPI_Handle_tp* pSPIHandle);

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
	while(len > 0)
	    {
	        //1. wait until TXE is set
	        while (SPI_GetFlagStatus(pSPIx, SPI_FLAG_TXE) == RESET);

	        //2. Check the DFF bit in the DR
	        if (pSPIx->CR1 & (1U << CR1_DFF))
	        {
	            //16 bits
	            pSPIx->DR = *((U16*)pRxBuffer);
	            len--;
				len--;
				(U16*)pRxBuffer++;
	        }
	        else
	        {
	            pSPIx->DR = *(pRxBuffer);
	            len--;
	            pRxBuffer++;
	        }
	    }
}

/**
 * IRQ Configuration and ISR handling
 */
void SPI_IRQIRConfig(U08 IRQNumber, U08 EnorDis)
{
    if(EnorDis == ENABLE)
    {
        if(IRQNumber <= 31)
        {
            *NVIC_ISER0 |= (1U << IRQNumber);
        }
        else if (IRQNumber > 31 && IRQNumber < 64)
        {
            *NVIC_ISER1 |= (1U << (IRQNumber % 32));
        }
        else if (IRQNumber >= 64 && IRQNumber < 96)
        {
            *NVIC_ISER2 |= (1U << (IRQNumber % 32));
        }
    }
    else
    {
        if(IRQNumber <= 31)
        {
            *NVIC_ICER0 |= (1U << IRQNumber);
        }
        else if (IRQNumber > 31 && IRQNumber < 64)
        {
            *NVIC_ICER1 |= (1U << (IRQNumber % 32));
        }
        else if (IRQNumber >= 64 && IRQNumber < 96)
        {
            *NVIC_ICER2 |= (1U << (IRQNumber % 32));
        }
    }
}

void SPI_IRQPriorityConfig(U08 IRQNumber, U08 IRQPriority)
{

}


void SPI_IRQHandling(SPI_Handle_tp* pSPIHandle)
{
    U08 temp1, temp2;
    // first let check for TXE
    temp1 = pSPIHandle->pSPIx->SR & (1 << SR_TXE);
    temp2 = pSPIHandle->pSPIx->SR & (1 << CR2_TXEIE);

    if(temp1 && temp2)
    {
        //handle TXE
        spi_txe_interrupt_handler(pSPIHandle);
    }

    // check for RXNE
    temp1 = pSPIHandle->pSPIx->SR & (1 << SR_RXNE);
    temp2 = pSPIHandle->pSPIx->SR & (1 << CR2_RXNEIE);

    if(temp1 && temp2)
    {
        //handle TXE
        spi_rxe_interrupt_handler(pSPIHandle);
    }

    //check for overrun flag
    temp1 = pSPIHandle->pSPIx->SR & (1 << SR_OVR);
    temp2 = pSPIHandle->pSPIx->SR & (1 << CR2_ERRIE);

    if(temp1 && temp2)
    {
        //handle ovr error
        spi_ovr_err_interrupt_handler(pSPIHandle);
    }
}

U08 SPI_DataTransferIT(SPI_Handle_tp* pSPIHandle, U08 *pTxBuffer, U32 len)
{
    U08 state = pSPIHandle->TxState;

    if(state != SPI_BUSY_IN_RX)
    {
        //1. save the TX buffer address and len information in some global variables
        pSPIHandle->pTxBuffer = pTxBuffer;
        pSPIHandle->TxLen = len;

        /**
         * 2. Mark the SPI state as busy in transmission so that no other
         * code can take over same SPI peripheral until transmission is over
         * */

        pSPIHandle->TxState = SPI_BUSY_IN_TX;

        //3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
        pSPIHandle->pSPIx->CR2 |= SPI_FLAG_TXE;
    
    }
        //4. Data Transmission will be handled by the ISR code (will implement later)  

    return state;
}

U08 SPI_DataReceiveIT(SPI_Handle_tp* pSPIHandle, U08 *pRxBuffer, U32 len)
{
    U08 state = pSPIHandle->RxState;

    if(state != SPI_BUSY_IN_TX)
    {
        //1. save the RX buffer address and len information in some global variables
        pSPIHandle->pRxBuffer = pRxBuffer;
        pSPIHandle->RxLen = len;

        /**
         * 2. Mark the SPI state as busy in transmission so that no other
         * code can take over same SPI peripheral until transmission is over
         * */

        pSPIHandle->RxState = SPI_BUSY_IN_TX;

        //3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
        pSPIHandle->pSPIx->CR2 |= SPI_FLAG_TXE;
     
    }
        //4. Data Transmission will be handled by the ISR code (will implement later) 

    return state;
}

/**
 * Implement helper function
 */
PRIVATE void spi_txe_interrupt_handler(SPI_Handle_tp* pSPIHandle)
{
    // Check the DFF bit in the DR
    if (pSPIHandle->pSPIx->CR1 & (1U << CR1_DFF))
    {
        //16 bits
        pSPIHandle->pSPIx->DR = *((U16*)pSPIHandle->pTxBuffer);
        pSPIHandle->TxLen -= 2;
        (U16*)pSPIHandle->pTxBuffer++;
    }
    else
    {
        pSPIHandle->pSPIx->DR = *(pSPIHandle->pTxBuffer);
        pSPIHandle->TxLen--;
        pSPIHandle->pTxBuffer++;
    }

    if(!pSPIHandle->TxLen)
    {
        /**
         * Txlen is zero, so close the spi communication and inform the application that Tx is over
         */

        // This prevents interrupts from setting up to TXE flag
        SPI_CloseTransmission(pSPIHandle);
        SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_TX_CMPL);
    }
}

PRIVATE void spi_rxe_interrupt_handler(SPI_Handle_tp* pSPIHandle)
{
    // Check the DFF bit in the DR
    if (pSPIHandle->pSPIx->CR1 & (1U << CR1_DFF))
    {
        //16 bits
        pSPIHandle->pSPIx->DR = *((U16*)pSPIHandle->pRxBuffer);
        pSPIHandle->RxLen -= 2;
        (U16*)pSPIHandle->pRxBuffer++;
    }
    else
    {
        pSPIHandle->pSPIx->DR = *(pSPIHandle->pRxBuffer);
        pSPIHandle->RxLen--;
        pSPIHandle->pRxBuffer++;
    }

    if(!pSPIHandle->RxLen)
    {
        /**
         * Txlen is zero, so close the spi communication and inform the application that Tx is over
         */

        // This prevents interrupts from setting up to TXE flag
        SPI_CloseReception(pSPIHandle);
        SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_RX_CMPL);
    }
}

PRIVATE void spi_ovr_err_interrupt_handler(SPI_Handle_tp* pSPIHandle)
{
    U08 temp = 0;
    // 1. clear the ovr flag

    if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
    {
        temp = pSPIHandle->pSPIx->DR;
        temp = pSPIHandle->pSPIx->SR;
        (void)temp;
    }
    // 2. inform the application 
    SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_OVR_ERR);
}

void SPI_CloseTransmission(SPI_Handle_tp* pSPIHandle)
{
    pSPIHandle->pSPIx->CR2 &= ~(1 << CR2_TXEIE);
    pSPIHandle->pTxBuffer = NULL;
    pSPIHandle->TxLen = 0;
    pSPIHandle->TxState = SPI_READY;

}

void SPI_CloseReception(SPI_Handle_tp* pSPIHandle)
{
    pSPIHandle->pSPIx->CR2 &= ~(1 << CR2_RXNEIE);
    pSPIHandle->pRxBuffer = NULL;
    pSPIHandle->RxLen = 0;
    pSPIHandle->RxState = SPI_READY;

}

void SPI_ClearOVRFlag(SPI_TypeDef* pSPIx)
{
    U08 temp;
    temp = pSPIx->DR;
    temp = pSPIx->SR;
    (void)temp;
}

__attribute__((weak)) void SPI_ApplicationEventCallBack(SPI_Handle_tp* pSPIHandle, U08 AppEv)
{
    //This is a weak implementation. the application may override this function
}
