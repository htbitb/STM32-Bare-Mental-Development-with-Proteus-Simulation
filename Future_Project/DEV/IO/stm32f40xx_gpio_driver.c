#include <stm32f40xx_gpio_driver.h>




 /**
  * Perpheral clock setup
  */
 void GPIO_PeriClockControl(U08 pGPIOx, U08 EnorDis)
 {
    if(EnorDis == ENABLE)
    {
        GPIOx_PCLOCK_EN(pGPIOx);
    }
    else
    {
    	//do nothing
    }
 }


/**
 * Init and De-init
 */
void GPIO_init(U08 pGPIOx, GPIO_Handle* pGPIOHandle)
{
    U32 tdata = 0;
    U08 temp_val[2] = {0, };
    /*1. Configure the mode of gpio pin*/
    if(pGPIOHandle->GPIO_PmapConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
        tdata = (pGPIOHandle->GPIO_PmapConfig.GPIO_PinMode << (2* pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber));
        pGPIOHandle->pGPIOx->MODER &=~(0x3 << pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber);
        pGPIOHandle->pGPIOx->MODER |= tdata;
    }
    else
    {
        /* GPIO Interrupt handler*/
        if(pGPIOHandle->GPIO_PmapConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
        {
            /*1. Configure the FTSR*/
            EXTI->FTSR |= (1U << pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber);

            /* Clear the corresponding RTSR bit for secure*/
            EXTI->RTSR &= ~(1U << pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber);
        }
        else if (pGPIOHandle->GPIO_PmapConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
        {
            /* Configure the RTST */
            EXTI->RTSR |= (1U << pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber);

            /* Clear the corresponding RTSR bit for secure*/
            EXTI->FTSR &= ~(1U << pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber);
        }
        else if (pGPIOHandle->GPIO_PmapConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
        {
            /*C Configure both FTSR and RTST*/
            EXTI->RTSR |= (1U << pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber);

            EXTI->FTSR |= (1U << pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber);

        }

        /* Configure the GPIO port selection in SYSCFG_EXTICR */
        SYSCGF_PCLK_EN();

        temp_val[0] = pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber / 4;
        temp_val[1] = pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber % 4;

        SYS_CFG->EXTICR[temp_val[0]] = pGPIOx << (temp_val[1] * 4);

        /* Enable the exti interrupt delivery using IMR*/
        EXTI->IMR |= (1U << pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber);
        
    }

    /*2. COnfigure the speed*/
    tdata = ((pGPIOHandle->GPIO_PmapConfig.GPIO_PinSpeed) << (2 * pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->OSPEEDR &=~(0x3 << pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber);
    pGPIOHandle->pGPIOx->OSPEEDR |= tdata;

    /*3. Configure the pupd settings*/
    tdata = ((pGPIOHandle->GPIO_PmapConfig.GPIO_PinPuPdControl) << (2 * pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->PUPDR &=~(0x3 << pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber);
    pGPIOHandle->pGPIOx->PUPDR |= tdata;

    /*4. Configure the optype*/
    tdata = ((pGPIOHandle->GPIO_PmapConfig.GPIO_PinOPType) << (pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->OTYPER &=~(0x1 << pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber);
    pGPIOHandle->pGPIOx->OTYPER |= tdata;

    /*5. configure the alternate function*/
    if(pGPIOHandle->GPIO_PmapConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
    {
        tdata = ((pGPIOHandle->GPIO_PmapConfig.GPIO_PinAltFunMode) << (4 * pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber));
        if(pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber <= GPIO_PIN7)
        {
            pGPIOHandle->pGPIOx->AFRL &=~(0xF << pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber);
            pGPIOHandle->pGPIOx->AFRL |= tdata;
        }
        else
        {
            pGPIOHandle->pGPIOx->AFRH &=~(0xF << pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber);
            pGPIOHandle->pGPIOx->AFRH |= tdata;
        }
    }

}
void GPIO_DeInit(U08 pGPIOx)
{
    GPIOx_REG_RESET(pGPIOx);
}

/**
 * Data Read and write
 */
U08 GPIO_ReadFromInputPin(GPIOx_TypeDef* pGPIOx, U08 PinNumber)
{
    U08 data;
    data = (U08)((pGPIOx->IDR >> PinNumber) & 0x01);

    return data;
}


U16 GPIO_ReadFromInputPort(GPIOx_TypeDef* pGPIOx)
{
    U16 data;
    data = (U16)(pGPIOx->IDR);

    return data;
}

void GPIO_WriteToOptPin(GPIOx_TypeDef* pGPIOx, U08 PinNumber, U08 Val)
{
    if(Val == SET)
    {
        pGPIOx->ODR |= (1U << PinNumber);
    }
    else
    {
        pGPIOx->ODR &= ~(1U << PinNumber);
    }
}


void GPIO_WriteToOptPort(GPIOx_TypeDef* pGPIOx, U16 Val)
{
    pGPIOx->ODR = Val;
}


void GPIO_ToggleOutputPin(GPIOx_TypeDef* pGPIOx, U08 PinNumber)
{
    pGPIOx->ODR ^= (1U << PinNumber);
}
/**
 * IRQ Configuration and ISR handling
 */
void GPIO_IRQIRConfig(U08 IRQNumber, U08 EnorDis)
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

void GPIO_IRQPriorityConfig(U08 IRQNumber, U08 IRQPriority)
{
    /* First lets find out tue IP register mapping*/
    U08 iprx = IRQNumber / 4;
    U08 iprx_section = IRQNumber % 4;

    U08 shift_amount = (8 * iprx_section) +  (8 - NO_PR_BITS_IMPLEMENTED);
    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

void GPIO_IRQHandling(U08 PinNumber)
{
    // clear the exit PR register corresponding to the pin number
    if(EXTI->PR & (1U << PinNumber))
    {
        EXTI->PR |= (1U << PinNumber);
    }
}
