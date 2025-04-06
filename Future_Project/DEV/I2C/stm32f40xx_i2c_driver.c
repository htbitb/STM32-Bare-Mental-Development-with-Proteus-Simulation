#include <stm32f40xx_i2c_driver.h>

/******************************************************************
 * Private function declare
*********************************************************************/
PRIVATE void I2C_GenerateStartCondition(I2C_TypeDef* pI2Cx);
PRIVATE void I2C_GenerateStopCondition(I2C_TypeDef* pI2Cx);
PRIVATE void I2C_ExecuteAddressPhase(I2C_TypeDef* pI2Cx, U32 SlaveAddr);
PRIVATE void I2C_ClearADDRFlage(I2C_TypeDef* pI2Cx);


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
    I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, SlaveAddr);

    //4. Confirm that address phase is completed by checking the ADDR flag in the SR1
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG(I2C_SR1_ADDR)));

    /**
     * 5. clear the ADDR flag according to its software sequence
     * Note: Until ADDR is cleared SCL will be stretched (Pull to LOW)
     */
    I2C_ClearADDRFlage(pI2CHandle->pI2Cx);

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

PRIVATE void I2C_GenerateStartCondition(I2C_TypeDef* pI2Cx)
{
    pI2Cx->I2C_CR1 |= (1 << I2C_CR1_START);
}

PRIVATE void I2C_GenerateStopCondition(I2C_TypeDef* pI2Cx)
{
    pI2Cx->I2C_CR1 |= (1 << I2C_CR1_STOP);
}

PRIVATE void I2C_ExecuteAddressPhase(I2C_TypeDef* pI2Cx, U32 SlaveAddr)
{
    SlaveAddr = SlaveAddr << 1;
    SlaveAddr &= ~(1); // SlaveAdrr is Slave address + r/nw bit=0
    pI2Cx->I2C_DR = SlaveAddr;
}

PRIVATE void I2C_ClearADDRFlage(I2C_TypeDef* pI2Cx)
{
    U32 dummyRead = pI2Cx->I2C_SR1;
    dummyRead = pI2Cx->I2C_SR2;
    (void)dummyRead;
}
/**
 * IRQ Configuration and ISR handling
 */
void I2C_IRQIRConfig(U08 IRQNumber, U08 EnorDis);
void I2C_IRQPriorityConfig(U08 IRQNumber, U08 IRQPriority);
void I2C_IRQHandling(I2C_Handle_tp* pI2CHandle);

void I2C_PeripheralControl(I2C_TypeDef* pI2Cx, U08 EnorDis);

U08 I2C_GetFlagStatus(I2C_TypeDef* pI2Cx, U32 FlagName)
{
    if(pI2Cx->I2C_SR1 & FlagName)
    {
        return FLAG_SET;
    } 
    return FLAG_RESET;
}


/**
 * Application callback
 */
void I2C_ApplicationEventCallBack(I2C_Handle_tp* pI2CHandle, U08 AppEv);
