#include <HAL.h>




 /**
  * Perpheral clock setup
  */
 void GPIO_PeriClockControl(U08 pGPIOx, U08 EnorDis)
 {
    if(EnorDis == ENABLE)
    {
        GPIOx_PCLOCK_EN(pGPIOx);
    }
 }


/**
 * Init and De-init
 */
void GPIO_init(GPIO_Handle* pGPIOHandle)
{
    U32 tdata = 0;
    /*1. Configure the mode of gpio pin*/
    if(pGPIOHandle->GPIO_PmapConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
        tdata = (pGPIOHandle->GPIO_PmapConfig.GPIO_PinMode << (2* pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber));
        pGPIOHandle->pGPIOx->MODER |= tdata;
    }
    else
    {
        // do nothing
    }

    /*2. COnfigure the speed*/
    tdata = ((pGPIOHandle->GPIO_PmapConfig.GPIO_PinSpeed) << (2 * pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->OSPEEDR |= tdata;

    /*3. Configure the pupd settings*/
    tdata = ((pGPIOHandle->GPIO_PmapConfig.GPIO_PinPuPdControl) << (2 * pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->PUPDR |= tdata;

    /*4. Configure the optype*/
    tdata = ((pGPIOHandle->GPIO_PmapConfig.GPIO_PinOPType) << (pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->OTYPER |= tdata;

    /*5. configure the alternate function*/
    if(pGPIOHandle->GPIO_PmapConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
    {
        tdata = ((pGPIOHandle->GPIO_PmapConfig.GPIO_PinAltFunMode) << (4 * pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber));
        if(pGPIOHandle->GPIO_PmapConfig.GPIO_PinNumber <= GPIO_PIN7)
        {
            pGPIOHandle->pGPIOx->AFRL |= tdata;
        }
        else
        {
            pGPIOHandle->pGPIOx->AFRH |= tdata;
        }
    }

}
void GPIO_DeInit(GPIOx_TypeDef* pGPIOx);

/**
 * Data Read and write
 */
U08 GPIO_ReadFromInputPin(GPIOx_TypeDef* pGPIOx, U08 PinNumber);
U16 GPIO_ReadFromInputPort(GPIOx_TypeDef* pGPIOx);
void GPIO_WriteToOptPin(GPIOx_TypeDef* pGPIOx, U08 PinNumber, U08 Val);
U16 GPIO_WriteToOptPort(GPIOx_TypeDef* pGPIOx, U16 Val);
void GPIO_ToggleOutputPin(void);

/**
 * IRQ Configuration and ISR handling
 */
void GPIO_IRQConfig(U08 IRQNumber, U08 IRQPriority, U08 EnorDis);
void GPIO_IRQHandling(U08 PinNumber);
