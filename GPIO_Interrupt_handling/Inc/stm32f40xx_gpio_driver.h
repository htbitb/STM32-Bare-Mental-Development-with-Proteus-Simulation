#ifndef INC_STM32F40XX_GPIO_H_
#define INC_STM32F40XX_GPIO_H_

#include <stm32f40xx.h>
#include <macro.h>

typedef uint8_t U08;
/**
 * This is a Configuration structure for a GPIO pin
 */

typedef struct
{
    U08 GPIO_PinNumber;
    U08 GPIO_PinMode;
    U08 GPIO_PinSpeed;
    U08 GPIO_PinPuPdControl;
    U08 GPIO_PinOPType;
    U08 GPIO_PinAltFunMode;
}GPIO_PinConfig;


/**
 * Handle structure for GPIO pin
 */
typedef struct 
{
    GPIOx_TypeDef* pGPIOx;              /*!< this holds the base address of the GPIO port to which the pin belongs*/
    GPIO_PinConfig GPIO_PmapConfig;     /*!< This holds GPIO pin configuration setting*/
}GPIO_Handle;


/**
 * GPIO pin possible mode
 */
#define GPIO_MODE_INPUT                 0
#define GPIO_MODE_OUTPUT                1   
#define GPIO_MODE_ALTFN                 2
#define GPIO_MODE_ANALOG                3   
#define GPIO_MODE_IT_FT                 4
#define GPIO_MODE_IT_RT                 5
#define GPIO_MODE_IT_RFT                6

/**
 * GPIO pin possible output types
 */
#define GPIO_OP_TYPE_PP                 0
#define GPIO_OP_TYPE_OD                 1

/**
 * GPIO pin possible output speeds
 */
#define GPIO_SPEED_LOW                  0
#define GPIO_SPEED_MED                  1
#define GPIO_SPEED_FAST                 2
#define GPIO_SPEED_HI                   3

/**
 * GPIO pin pull up and pull down configuration macros
 */
#define GPIO_NO_PUPD                    0
#define GPIO_PIN_PU                     1
#define GPIO_PIN_PD                     2

/**
 * Enum for pin number of each port
 */
enum
{
    GPIO_PIN0,
    GPIO_PIN1,
    GPIO_PIN2,
    GPIO_PIN3,
    GPIO_PIN4,
    GPIO_PIN5,
    GPIO_PIN6,
    GPIO_PIN7,
    GPIO_PIN8,
    GPIO_PIN9,
    GPIO_PIN10,
    GPIO_PIN11,
    GPIO_PIN12,
    GPIO_PIN13,
    GPIO_PIN14,
    GPIO_PIN15,
};

/***********************************************************************************************************
 *                                     APIs supported for this driver
 **********************************************************************************************************/

 /**
  * Perpheral clock setup
  */
 void GPIO_PeriClockControl(U08 pGPIOx, U08 EnorDis);

/**
 * Init and De-init
 */
void GPIO_init(U08 pGPIOx, GPIO_Handle* pGPIOHandle);
void GPIO_DeInit(U08 pGPIOx);

/**
 * Data Read and write
 */
U08 GPIO_ReadFromInputPin(GPIOx_TypeDef* pGPIOx, U08 PinNumber);
U16 GPIO_ReadFromInputPort(GPIOx_TypeDef* pGPIOx);
void GPIO_WriteToOptPin(GPIOx_TypeDef* pGPIOx, U08 PinNumber, U08 Val);
void GPIO_WriteToOptPort(GPIOx_TypeDef* pGPIOx, U16 Val);
void GPIO_ToggleOutputPin(GPIOx_TypeDef* pGPIOx, U08 PinNumber);

/**
 * IRQ Configuration and ISR handling
 */
void GPIO_IRQIRConfig(U08 IRQNumber, U08 EnorDis);
void GPIO_IRQPriorityConfig(U08 IRQNumber, U08 IRQPriority);
void GPIO_IRQHandling(U08 PinNumber);




































#endif

