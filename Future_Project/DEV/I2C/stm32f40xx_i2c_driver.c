#include <stm32f40xx_i2c_driver.h>

/******************************************************************
 * Private function declare
*********************************************************************/
PRIVATE void I2C_GenerateStartCondition(I2C_TypeDef* pI2Cx);
PRIVATE void I2C_GenerateStopCondition(I2C_TypeDef* pI2Cx);
PRIVATE void I2C_ExecuteAddressPhase(I2C_TypeDef* pI2Cx, U32 SlaveAddr, bool RW);
PRIVATE void I2C_ClearADDRFlage(I2C_Handle_tp* pI2CHandler);


/******************************************************************* */
U32 const AHB_PreScale[8]   = {2, 4, 6, 16, 64, 128, 256, 512};
U32 const APB1_PreScale[4]  = {2, 4, 8, 16};

U32 RCC_GetPLLOutputClock(void)
{
 // implement later
 return 0;
}

U32 RCC_GetPCLK1Value(void)
{
    U32 PCLK1, SysClk, AHBP_PreSc, APB1_PreSc;
    U08 clksrc, temp;

    clksrc = (RCC->CFGR >> 2) & 3U;

    if(clksrc == 0)
    {
        SysClk = 16000000;
    }
    else if (clksrc == 1)
    {
        SysClk = 8000000;
    }
    else
    {
        SysClk = RCC_GetPLLOutputClock();
    }

    // AHP prescale
    temp = ((RCC->CFGR >> 4) & 0xF);
    if(temp < 8 )
    {
        AHBP_PreSc = 1;
    } 
    else
    {
        AHBP_PreSc = AHB_PreScale[temp - 8];
    }

    // APB1 prescale
    temp = ((RCC->CFGR >> 10) & 0x7);
    if(temp < 4 )
    {
        APB1_PreSc = 1;
    } 
    else
    {
        APB1_PreSc = APB1_PreScale[temp - 8];
    }

    PCLK1 = (SysClk / AHBP_PreSc) / APB1_PreSc;

    return PCLK1;
}
 
/**
 * Peripheral clock setup
 */
void I2Cx_PeriClockControl(U08 I2Cx, U08 EnorDis)
{
    if(EnorDis == ENABLE)
    {
        I2Cx_PCLOCK_EN(I2Cx);
    }
    else
    {
        //do nothing
    }
}

/**
 * Init and De-init
 */
void I2Cx_init(U08 I2Cx, I2C_Handle_tp* pI2CHandle)
{
    U32 tempreg = 0;
    U08 Trise = 0;
    /*1. ACK control bit*/
    tempreg |= (pI2CHandle->I2CConf.I2C_ACKControl << I2C_CR1_ACK);
    pI2CHandle->pI2Cx->I2C_CR1 = tempreg;

    /*2. configure the FREQ field of CR2*/
    tempreg = 0;
    tempreg = RCC_GetPCLK1Value() / 1000000U;
    pI2CHandle->pI2Cx->I2C_CR2 = (tempreg & 0x3F);

    /*3. program the device own address*/
    tempreg = pI2CHandle->I2CConf.I2C_DeviceAddr << 1;
    pI2CHandle->pI2Cx->I2C_OAR1 = tempreg;

    /*4. CCR Calculations*/
    U16 ccr_value = 0;
    tempreg = 0;
    if(pI2CHandle->I2CConf.I2C_SLCSpeed <= I2C_SCL_SPEED_SM)
    {
        //mode standard
        ccr_value = RCC_GetPCLK1Value() / (2 * pI2CHandle->I2CConf.I2C_SLCSpeed);
    }
    else
    {
        //fast mode
        tempreg |= (1 << 15);
        tempreg |= (pI2CHandle->I2CConf.I2C_FMDutyCycle << 14);

        if(pI2CHandle->I2CConf.I2C_FMDutyCycle == I2C_FM_DUTY_2)
        {
            ccr_value = RCC_GetPCLK1Value() / (3 * pI2CHandle->I2CConf.I2C_SLCSpeed);
        }
        else
        {
            ccr_value = RCC_GetPCLK1Value() / (25 * pI2CHandle->I2CConf.I2C_SLCSpeed);
        }
        tempreg |= (ccr_value & 0xFFF);
    }
    pI2CHandle->pI2Cx->I2C_CCR = tempreg;

    /*TRISE configuration */
    if(pI2CHandle->I2CConf.I2C_SLCSpeed <= I2C_SCL_SPEED_SM)
    {
        //mode standard
        Trise = (RCC_GetPCLK1Value() / 1000000U) + 1;
    }
    else
    {
        Trise = ((RCC_GetPCLK1Value() * 300) / 1000000U) + 1;
    }
    pI2CHandle->pI2Cx->I2C_TRISE |= (Trise & 0x3F);
}
void I2Cx_DeInit(U08 I2Cx)
{

}

/**
 * Data Send and Receive
 */
void I2C_MasterSendData(I2C_Handle_tp *pI2CHandle, U08* pTxbuffer, U32 len, U08 SlaveAddr)
{
    //1. Generate start condition
    I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

    /**
     * 2. Confirm that start generation is completed by checking the  SB flag in the SR1
     * Note: Until SB is cleared SCL will be stretched (pulled to low)
     */
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG(I2C_SR1_SB)));

    //3. Send the address of the slave with r/nw bit set to W(0) (total 8 bits)
    I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, SlaveAddr, write);

    //4. Confirm that address phase is completed by checking the ADDR flag in the SR1
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG(I2C_SR1_ADDR)));

    /**
     * 5. clear the ADDR flag according to its software sequence
     * Note: Until ADDR is cleared SCL will be stretched (Pull to LOW)
     */
    I2C_ClearADDRFlage(pI2CHandle);

    //6. send the data until len becomes 0
    while(len > 0)
    {
        while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG(I2C_SR1_TxE))); //wait until TxE is set
        pI2CHandle->pI2Cx->I2C_DR = *pTxbuffer;
        pTxbuffer++;
        len--;
    }
    /**
     * 7. When len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
     * Note: TXE=1, BTF=1, means that both SR anf DR are empty and next transmission should begin
     * when BTF=1 SCL will be stretched (pulled to LOW)
     */
    while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG(I2C_SR1_TxE)));

    while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG(I2C_SR1_BTF)));


    /**
     * 8. Generate STOP  condition and master need not to wait for the completion of stop condition
     * Note: generating STOP, automatically clears the BTF
     */
    I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
}

void I2C_MasterReceiveData(I2C_Handle_tp* pI2CHandle, U08 *pRxBuffer, U08 len, U08 SlaveAddr)
{
    //1. Generate the Start condition
    I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

    /**
     * 2. Confirm that start generation is completed by checking the SB flag in the SR1
     * Note: Until SB is cleared SCL will be stretched (pulled to low)
     */
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG(I2C_SR1_SB)));

    //3. Send the address of the slave with r/w but set to R(1) (total 8 bits)
    I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, SlaveAddr, read);

    //4. wait until address phase is completed by checking the ADDR flag in the SR1
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG(I2C_SR1_ADDR)));

    //procedure to read only 1 byte from slave
    if(len == 1)
    {
        //Disable Acking
        I2C_ACKManage(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

        // generate STOP condition
        I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

        //clear the ADDR flag
        I2C_ClearADDRFlage(pI2CHandle);
        
        //wait until RxE is set 
        while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG(I2C_SR1_RxNE)));

        //read data in to buffer
        *pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;

        return;
    }

    /**
     * Procedure to receive more data
     */
    if (len > 1)
    {
        // clear ADDR flag
        I2C_ClearADDRFlage(pI2CHandle);

        //read data until len become zeros
        for(U32 i = len; i >= 0; i--)
        {
            //wait until RxNe is set
            while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG(I2C_SR1_RxNE)));

            if(len == 2) // last 
            {
                // clear ack bit
                I2C_ACKManage(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

                // generate stop bit
                I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

            }

            // read data from buffer
            *pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
            pRxBuffer++;
        }
    }

    //re-enable Acking
    I2C_ACKManage(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
    
}

PRIVATE void I2C_GenerateStartCondition(I2C_TypeDef* pI2Cx)
{
    pI2Cx->I2C_CR1 |= (1 << I2C_CR1_START);
}

PRIVATE void I2C_GenerateStopCondition(I2C_TypeDef* pI2Cx)
{
    pI2Cx->I2C_CR1 |= (1 << I2C_CR1_STOP);
}

PRIVATE void I2C_ExecuteAddressPhase(I2C_TypeDef* pI2Cx, U32 SlaveAddr, bool RW)
{
    if(RW == 0)
    {
        SlaveAddr = SlaveAddr << 1;
        SlaveAddr &= ~(1); // SlaveAdrr is Slave address + r/nw bit=0
    }
    else
    {
        SlaveAddr = SlaveAddr << 1;
        SlaveAddr |= 1U; // SlaveAdrr is Slave address + r/nw bit=1
    }
    pI2Cx->I2C_DR = SlaveAddr;
    
}

PRIVATE void I2C_ClearADDRFlage(I2C_Handle_tp* pI2CHandler)
{
    U32 dummyRead;
    // Check for device mode
    if(pI2CHandler->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL))
    {
        // device mode is in master mode
        if(pI2CHandler->TxRxState == I2C_BUSY_IN_RX)
        {
            if(pI2CHandler->RxSize == 1)
            {
                //first disable the ack
                I2C_ACKManage(pI2CHandler->pI2Cx, DISABLE);

                // clear addr flag
                dummyRead = pI2CHandler->pI2Cx->I2C_SR1;
                dummyRead = pI2CHandler->pI2Cx->I2C_SR2;
                (void)dummyRead;
            }
        }
        else
        {
            dummyRead = pI2CHandler->pI2Cx->I2C_SR1;
            dummyRead = pI2CHandler->pI2Cx->I2C_SR2;
            (void)dummyRead;
        }
    }
    else
    {
        dummyRead = pI2CHandler->pI2Cx->I2C_SR1;
        dummyRead = pI2CHandler->pI2Cx->I2C_SR2;
        (void)dummyRead;
    }

    
}

/**
 * I2C interrupt handler
 */
void I2C_IRQPriorityConfig(U08 IRQNumber,U32 IRQPriority)
{
    //1. first lets find out the ipr register
	U08 iprx = IRQNumber / 4;
	U08 iprx_section  = IRQNumber %4 ;

	U08 shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;

	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );
}

void I2C_IRQInterruptConfig(U08 IRQNumber, U32 EnorDis)
{
    if(EnorDis == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//program ISER0 register
			*NVIC_ISER0 |= ( 1 << IRQNumber );

		}else if(IRQNumber > 31 && IRQNumber < 64 ) //32 to 63
		{
			//program ISER1 register
			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ISER2 register //64 to 95
			*NVIC_ISER3 |= ( 1 << (IRQNumber % 64) );
		}
	}else
	{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			*NVIC_ICER0 |= ( 1 << IRQNumber );
		}else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			//program ICER1 register
			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 6 && IRQNumber < 96 )
		{
			//program ICER2 register
			*NVIC_ICER3 |= ( 1 << (IRQNumber % 64) );
		}
	}

}

U08 I2C_MasterSendDataIT(I2C_Handle_tp *pI2CHandle, U08* pTxbuffer, U32 len, U08 SlaveAddr, U08 Rs)
{
    uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pTxBuffer = pTxbuffer;
		pI2CHandle->Txlen = len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Rs = Rs;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITERREN);

	}

	return busystate;
}

U08 I2C_MasterReceiveDataIT(I2C_Handle_tp* pI2CHandle, U08 *pRxBuffer, U08 len, U08 SlaveAddr, U08 Rs)
{
    uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->Rxlen = len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = len; //Rxsize is used in the ISR code to manage the data reception 
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Rs = Rs;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITERREN);
		
	}

	return busystate;
}
/**
 * IRQ Configuration and ISR handling
 */

void I2C_EV_IRQHandling(I2C_Handle_tp* pI2CHandle)
{
    //Interrupt handling for both master and slave mode of a device
    U32 temp1, temp2, temp3;

    temp1 = pI2CHandle->pI2Cx->I2C_CR2 & (1 << I2C_CR2_ITEVTEN);
    temp2 = pI2CHandle->pI2Cx->I2C_CR2 & (1 << I2C_CR2_ITBUFEN);

    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_SB);
    
	//1. Handle For interrupt generated by SB event
	//	Note : SB flag is only applicable in Master mode
    if(temp1 && temp3)
    {
        /**
         * the interrupt is generated because of SB
         * this block will not be executed in slave mode because for slave SB alway 0
         * Let declare for address phase
         */
        if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
        {
            I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, pI2CHandle->DevAddr, write);
        }
        else if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
        {
            I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, pI2CHandle->DevAddr, read);
        }
    }
	
    
	/*2. Handle For interrupt generated by ADDR event 
	*Note : When master mode : Address is sent 
	* When Slave mode   : Address matched with own address 
    */
    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_ADDR);
    if(temp1 && temp3)
    {
        //ADDR is set
        I2C_ClearADDRFlage(pI2CHandle);
    }

	//3. Handle For interrupt generated by BTF(Byte Transfer Finished) event  
    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_BTF);
    if(temp1 && temp3)
    {
        //Stopf is saved
        if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
        {
            //make sure that TXE is also set
            if (I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_TxE))
            {
                //BTF, TXE = 1
                if(pI2CHandle->Txlen == 0)
                {
                    //1. Generate the stop condition
                    if(pI2CHandle->Rs == I2C_DISABLE_RS)
                        I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

                    //2. reset all the member elements of the handle structure
                    I2C_CloseSendData(pI2CHandle);

                    //3. Notify the application about transmission complete
                    I2C_ApplicationEventCallBack(pI2CHandle, I2C_EV_TX_CMPLT);
                }
            }
        }
        else if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
        {
            ;
        }
    }
	
	/*4. Handle For interrupt generated by STOPF event 
	* Note : Stop detection flag is applicable only slave mode . For master this flag will never be set
	*/
    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_STOPF);
    if(temp1 && temp3)
    {
        pI2CHandle->pI2Cx->I2C_CR1 |= 0x0000;

        // Notify the application that STOP is detected
        I2C_ApplicationEventCallBack(pI2CHandle, I2C_EV_STOP);
    }

	//5. Handle For interrupt generated by TXE event 
    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_TxE);
    if(temp1 && temp2 &&temp3)
    {
        //TXE is set
        //We have to do the data transmission
        if(pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL))
        {
            if(pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL))
            {
                I2C_MasterHanldeTXEInterrupt(pI2CHandle);
            }
        }
        else
        {
            //slave
			//make sure that the slave is really in transmitter mode
		    if(pI2CHandle->pI2Cx->I2C_SR2 & ( 1 << I2C_SR2_TRA))
		    {
		    	I2C_ApplicationEventCallBack(pI2CHandle,I2C_EV_DATA_REQ);
		    }
        }
    }
	
	//6. Handle For interrupt generated by RXNE even
    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_RxNE);
    if(temp1 && temp2 &&temp3)
    {
        //RXNE is set
        //check for device mode
        if(pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL))
        {
            if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
            {
                I2C_MasterHanldeRXENInterrupt(pI2CHandle);
            }
        }
        else
        {
            //slave
			//make sure that the slave is really in transmitter mode
		    if(pI2CHandle->pI2Cx->I2C_SR2 & ( 1 << I2C_SR2_TRA))
		    {
		    	I2C_ApplicationEventCallBack(pI2CHandle,I2C_EV_DATA_RCV);
		    }
        }
    }
}

void I2C_MasterHanldeRXENInterrupt(I2C_Handle_tp* pI2CHandle)
{
    if(pI2CHandle->RxSize == 1)
    {
        *pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
        pI2CHandle->pRxBuffer--;
    }

    if(pI2CHandle->RxSize > 1)
    {
        if(pI2CHandle->Rxlen == 2)
        {
            //clear the ack bit
            I2C_ACKManage(pI2CHandle->pI2Cx, DISABLE);
        }

        //read DR
        *pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
        pI2CHandle->pRxBuffer--;
        pI2CHandle->Rxlen--;
    }

    if(pI2CHandle->Rxlen == 0)
    {
        //close the I2C data reception and notify the application

        //1. close I2C rx
        I2C_CloseReceiveData(pI2CHandle);

        //2. Notify
        I2C_ApplicationEventCallBack(pI2CHandle, I2C_EV_RX_CMPLT);
    }
}

void I2C_MasterHanldeTXEInterrupt(I2C_Handle_tp* pI2CHandle)
{
    if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
    {
        if(pI2CHandle->Txlen > 0)
        {
            //1. load the data in to DR
            pI2CHandle->pI2Cx->I2C_DR = *(pI2CHandle->pTxBuffer);

            //2. decrement the Txlen
            pI2CHandle->Txlen--;

            //3. Increment the buffer address
            pI2CHandle->pTxBuffer++;
        }
    }
}

void I2C_CloseReceiveData(I2C_Handle_tp *pI2CHandler)
{
    //Implement the code to disable ITBUFEN control bit
    pI2CHandler->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITBUFEN);

    //Implement the code to disable ITEVFEN control bit
    pI2CHandler->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITEVTEN);

    pI2CHandler->TxRxState = I2C_READY;
    pI2CHandler->pRxBuffer = NULL;
    pI2CHandler->Rxlen = 0;
    pI2CHandler->RxSize = 0;

    if(pI2CHandler->I2CConf.I2C_ACKControl == I2C_ACK_ENABLE)
    {
        I2C_ACKManage(pI2CHandler->pI2Cx, ENABLE);
    }
}

void I2C_CloseSendData(I2C_Handle_tp *pI2CHandler)
{
    //Implement the code to disable ITBUFEN control bit
    pI2CHandler->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITBUFEN);

    //Implement the code to disable ITEVFEN control bit
    pI2CHandler->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITEVTEN);

    pI2CHandler->TxRxState = I2C_READY;
    pI2CHandler->pTxBuffer = NULL;
    pI2CHandler->Txlen = 0;
    I2C_ACKManage(pI2CHandler->pI2Cx, ENABLE);
}

void I2C_ACKManage(I2C_TypeDef* pI2Cx, U08 EnorDis)
{
    if(EnorDis == I2C_ACK_ENABLE)
	{
		//enable the ack
		pI2Cx->I2C_CR1 |= ( 1 << I2C_CR1_ACK);
	}else
	{
		//disable the ack
		pI2Cx->I2C_CR1 &= ~( 1 << I2C_CR1_ACK);
	}
}

void I2C_ER_IRQHandling(I2C_Handle_tp* pI2CHandle)
{
    uint32_t temp1,temp2;

    //Know the status of  ITERREN control bit in the CR2
	temp2 = (pI2CHandle->pI2Cx->I2C_CR2) & ( 1 << I2C_CR2_ITERREN);


/***********************Check for Bus error************************************/ 
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1<< I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error
		
		//Implement the code to clear the buss error flag 
		pI2CHandle->pI2Cx->I2C_SR1 &= ~( 1 << I2C_SR1_BERR);
		
		//Implement the code to notify the application about the error 
		I2C_ApplicationEventCallBack(pI2CHandle, I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/ 
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		//This is arbitration lost error
		
		//Implement the code to clear the arbitration lost error flag
        pI2CHandle->pI2Cx->I2C_SR1 &= ~( 1 << I2C_SR1_ARLO);
		
		//Implement the code to notify the application about the error 
        I2C_ApplicationEventCallBack(pI2CHandle,I2C_ERROR_ARLO);
	   
	}

/***********************Check for ACK failure  error************************************/ 

	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error
		
	    //Implement the code to clear the ACK failure error flag
        pI2CHandle->pI2Cx->I2C_SR1 &= ~( 1 << I2C_SR1_AF);
		
		//Implement the code to notify the application about the error
        I2C_ApplicationEventCallBack(pI2CHandle,I2C_ERROR_AF); 
	}

/***********************Check for Overrun/underrun error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun
		
	    //Implement the code to clear the Overrun/underrun error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~( 1 << I2C_SR1_OVR);
		
		//Implement the code to notify the application about the error
        I2C_ApplicationEventCallBack(pI2CHandle,I2C_ERROR_OVR); 
	}

/***********************Check for Time out error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error
		
	    //Implement the code to clear the Time out error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~( 1 << I2C_SR1_TIMEOUT);
		
		//Implement the code to notify the application about the error
        I2C_ApplicationEventCallBack(pI2CHandle,I2C_ERROR_TIMEOUT);
	}
}

U08 I2C_GetFlagStatus(I2C_TypeDef* pI2Cx, U32 FlagName)
{
    if(pI2Cx->I2C_SR1 & FlagName)
    {
        return FLAG_SET;
    } 
    return FLAG_RESET;
}

void I2C_SlaveSendData(I2C_TypeDef *pI2Cx,U08 data)
{
	pI2Cx->I2C_DR = data;
}

U08 I2C_SlaveReceiveData(I2C_TypeDef *pI2Cx)
{
    return (U08) pI2Cx->I2C_DR;
}


 /**
  * Application callback
  */
void I2C_ApplicationEventCallBack(I2C_Handle_tp* pI2CHandle, U08 AppEv)
{
	// implement later

}
