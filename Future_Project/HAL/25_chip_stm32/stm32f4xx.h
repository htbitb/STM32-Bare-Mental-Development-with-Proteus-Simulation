#ifndef INC_STM32F40XX_H_
#define INC_STM32F40XX_H_

#include "Future_Project\SYS\macros.h"

/*********************** START: Processor Specific Details ********************* */
/**
 *  ARM Cortex M4 Processor NVIC ISERx register Address
 */
#define NVIC_ISER0				((__IO U32*)0xE000E100)
#define NVIC_ISER1				((__IO U32*)0xE000E104)
#define NVIC_ISER2				((__IO U32*)0xE000E108)
#define NVIC_ISER3				((__IO U32*)0xE000E10C)
#define NVIC_ISER4				((__IO U32*)0xE000E110)
#define NVIC_ISER5				((__IO U32*)0xE000E114)
#define NVIC_ISER6				((__IO U32*)0xE000E118)
#define NVIC_ISER7				((__IO U32*)0xE000E11C)

/**
 * ARM Cortex M4 Processor NVIC ICERx register Address
 */
#define NVIC_ICER0				((__IO U32*)0xE000E180)
#define NVIC_ICER1				((__IO U32*)0xE000E184)
#define NVIC_ICER2				((__IO U32*)0xE000E188)
#define NVIC_ICER3				((__IO U32*)0xE000E18C)
#define NVIC_ICER4				((__IO U32*)0xE000E190)
#define NVIC_ICER5				((__IO U32*)0xE000E194)
#define NVIC_ICER6				((__IO U32*)0xE000E198)
#define NVIC_ICER7				((__IO U32*)0xE000E19C)

/**
 * 
 */
#define NVIC_PR_BASE_ADDR		((__IO U32*)0xE000E400)


#define NO_PR_BITS_IMPLEMENTED	4
/**
 * Declare Base address of Flash and SRAM memories
 */

#define FLASH_BASE				(0x08000000UL) 	/*This is the start address of Flash memory*/
#define SRAM1_BASE				(0x20000000UL)	/*This is the start address of SRAM1 memory*/
#define SRAM2_BASE				(0x20001C00UL)	/*This is the start address of SRAM2 memory = SRAM1 + 112KB*/
#define	SRAM					(SRAM1_BASE)
#define ROM						(0x1FFF0000UL)


/**
 * AHBx and APBx Bus Peripheral base address
 */

#define PERIPH_BASE				(0x40000000UL)
#define APB1PERIPH_BASE			(PERIPH_BASE)
#define APB2PERIPH_BASE			(PERIPH_BASE + 0x10000UL) 		/*!<Plus the offset of APB2 Peripheral*/
#define AHB1PERIPH_BASE			(PERIPH_BASE + 0x20000UL) 		/*!<Plus the offset of AHB1 Peripheral*/
#define AHB2PERIPH_BASE			(PERIPH_BASE + 0x10000000UL) 	/*!<Plus the offset of AHB2 Peripheral*/


/**
 * Base address of peripherals which are hanging on AHB1 bus
 */

#define GPIOA_BASE				(AHB1PERIPH_BASE)
#define GPIOB_BASE				(AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE				(AHB1PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE				(AHB1PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE				(AHB1PERIPH_BASE + 0x1000UL)
#define GPIOH_BASE				(AHB1PERIPH_BASE + 0x1C00UL)

#define RCC_BASE				(AHB1PERIPH_BASE + 0x3800UL)

/**
 * Base address of peripherals which are hanging on APB1 bus
 */

#define TIM2_BASE				(APB1PERIPH_BASE)
#define TIM3_BASE				(APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE				(APB1PERIPH_BASE + 0x0800UL)
#define TIM5_BASE				(APB1PERIPH_BASE + 0x0C00UL)
#define I2S2_BASE				(APB1PERIPH_BASE + 0x3400UL)
#define SPI2_BASE				(APB1PERIPH_BASE + 0x3800UL)
#define SPI3_BASE				(APB1PERIPH_BASE + 0x3C00UL)
#define USART2_BASE				(APB1PERIPH_BASE + 0x4400UL)
#define I2C1_BASE				(APB1PERIPH_BASE + 0x5400UL)
#define I2C2_BASE				(APB1PERIPH_BASE + 0x5800UL)
#define I2C3_BASE				(APB1PERIPH_BASE + 0x5C00UL)

/**
 * Base address of peripherals which are hanging on APB1 bus
 */

#define TIM1_BASE				(APB2PERIPH_BASE)
#define USART1_BASE				(APB2PERIPH_BASE + 0x1000UL)
#define USART6_BASE				(APB2PERIPH_BASE + 0x1400UL)
#define ADC1_BASE				(APB2PERIPH_BASE + 0x2000UL)
#define SPI1_BASE				(APB2PERIPH_BASE + 0x3000UL)
#define SPI4_BASE				(APB2PERIPH_BASE + 0x3400UL)
#define SYSCFG_BASE				(APB2PERIPH_BASE + 0x3800UL)
#define EXTI_BASE				(APB2PERIPH_BASE + 0x3C00UL)



/***********************peripheral register definition structures ***********************/

/**
 * Peripheral register definition structure for GPIO
 */

typedef struct
{
	__IO U32 MODER;
	__IO U32 OTYPER;
	__IO U32 OSPEEDR;
	__IO U32 PUPDR;
	__IO U32 IDR;
	__IO U32 ODR;
	__IO U32 BSRR;
	__IO U32 LCKR;
	__IO U32 AFRL;
	__IO U32 AFRH;
}GPIOx_TypeDef, *GPIOx_TypeDef_ptr;

/**
 * RCC register definition structure
 */
typedef struct
{
	__IO U32 CR;
	__IO U32 PLLCFG;
	__IO U32 CFGR;
	__IO U32 CIR;
	__IO U32 AHB1RSTR;
	__IO U32 AHB2RSTR;
	__IO U32 reserver1[2];
	__IO U32 APB1RSTR;
	__IO U32 APB2RSTR;
	__IO U32 reserver2[2];
	__IO U32 AHB1ENR;
	__IO U32 AHB2ENR;
	__IO U32 reserver3[2];
	__IO U32 APB1ENR;
	__IO U32 APB2ENR;
	__IO U32 reserver4[2];
	__IO U32 AHB1LPENR;
	__IO U32 AHB2LPENR;
	__IO U32 reserver5[2];
	__IO U32 APB1LPENR;
	__IO U32 APB2LPENR;
	__IO U32 reserver6[2];
	__IO U32 BDCR;
	__IO U32 CSR;
	__IO U32 reserver7[2];
	__IO U32 SSCGR;
	__IO U32 PLLI2SCFGR;
	__IO U32 reserver8;
	__IO U32 DCKCFGR;
}RCC_TypeDef, *RCC_TypeDef_ptr;


/**
 * EXTI register definition structure
 */
typedef struct
{
	__IO U32 IMR;
	__IO U32 EMR;
	__IO U32 RTSR;
	__IO U32 FTSR;
	__IO U32 SWIER;
	__IO U32 PR;
}EXTI_TypeDef;
 

/**
 * Peripheral register definition structure for SYSCFG
 */
typedef struct 
{
	__IO U32 MEMRMP;
	__IO U32 PMC;
	__IO U32 EXTICR[4];
	__IO U32 DUMMY[2];
	__IO U32 CMPCR;
}SYSCFG_TypeDef;

/**
 * Peripheral register definition structure for SPIx
 */
typedef struct 
{
    __IO U32   SPI_CR1;
    __IO U32   SPI_CR2;
    __IO U32   SPI_SR;
    __IO U32   SPI_DR;
    __IO U32   SPI_CRCPR;
    __IO U32   SPI_RXCRCR;
    __IO U32   SPI_TXCRCR;
    __IO U32   SPI_I2SCFGR;
    __IO U32   SPI_I2SPR;
}SPI_TypeDef;

/**
 * Peipheral register definition structure for I2C
 */
typedef struct
{
	__IO U32	I2C_CR1;
	__IO U32	I2C_CR2;
	__IO U32	I2C_OAR1;
	__IO U32	I2C_OAR2;
	__IO U32	I2C_DR;
	__IO U32	I2C_SR1;
	__IO U32	I2C_SR2;
	__IO U32	I2C_CCR;
	__IO U32	I2C_TRISE;
	__IO U32	I2C_FLTR;
}I2C_TypeDef;



/********************** enum for macros used ************************/
enum
{
	TIM2 = 0,
	TIM3,
	TIM4,
	TIM5,
	WWDGEN = 11,
	SPI_2 = 14,
	SPI_3,
	USART2 = 17,
	I2C1 = 21,
	I2C2,
	I2C3,
	PWREN = 28,
};

enum
{
	TIM1 = 0,
	USART1 = 4,
	USART6,
	ADC1 = 8,
	SDIO = 11,
	SPI_1,
	SPI_4,
	SYSCFG,
	TIM9 = 16,
	TIM10,
	TIM11,
};

enum
{
	GPIO_A = 0,
	GPIO_B,
	GPIO_C,
	GPIO_D,
	GPIO_E,
	GPIO_H = 7,
	CRCLP = 12,
	DMA1 = 21,
	DMA2,
};


/**********************  enum *********************************** */

/**
 * peripheral definition (Peripheral base address typecasts to GPIO register)
 */
#define GPIOA 					((GPIOx_TypeDef*) GPIOA_BASE)
#define GPIOB 					((GPIOx_TypeDef*) GPIOB_BASE)
#define GPIOC 					((GPIOx_TypeDef*) GPIOC_BASE)
#define GPIOD 					((GPIOx_TypeDef*) GPIOD_BASE)
#define GPIOE 					((GPIOx_TypeDef*) GPIOE_BASE)
#define GPIOH 					((GPIOx_TypeDef*) GPIOH_BASE)

#define RCC						((RCC_TypeDef*) RCC_BASE)
#define EXTI					((EXTI_TypeDef*) EXTI_BASE)
#define SYS_CFG					((SYSCFG_TypeDef*)SYSCFG_BASE)

#define SPI1					((SPI_TypeDef*) SPI1_BASE)
#define SPI2					((SPI_TypeDef*) SPI2_BASE)
#define SPI3					((SPI_TypeDef*) SPI3_BASE)
#define SPI4					((SPI_TypeDef*) SPI4_BASE)

/**
 * IRQ (Interrupt Request)
 */
#define IRQ_NO_EXTI0		6
#define IRQ_NO_EXTI1		7
#define IRQ_NO_EXTI2		8
#define IRQ_NO_EXTI3		9
#define IRQ_NO_EXTI4		10
#define IRQ_NO_EXTI9_5		23
#define IRQ_NO_EXTI15_10	40
#define IRQ_NO_EXTI16		1
#define IRQ_NO_EXTI17		41
#define IRQ_NO_EXTI18		42
#define IRQ_NO_EXTI21		2
#define IRQ_NO_EXTI22		3



/**
 * Clock Enable Macros for GPIOx peripherals
 */
#define GPIOx_PCLOCK_EN(pmap)	do{(RCC->AHB1ENR |= (1U<<pmap));}while(0)

/**
 * Clock enable Macros for I2Cx peripherals
 */
#define I2Cx_PCLOCK_EN(pmap)	do{(RCC->APB1ENR |= (1U<<pmap));}while(0)

/**
 * Clock enable Macros for SPIx peripherals
 */
#define SPIx_PCLOCK_EN(pmap)		do{										\
											if((pmap == 12 || pmap == 13))		\
											{									\
												RCC->APB2ENR |= (1U<<pmap);		\
											}									\
											else								\
											{									\
												RCC->APB1ENR |= (1U<<pmap);		\
											}									\
										} while(0)

/*
 * Clock enable Macros for USART
 */
#define USARTx_PCCK_EN(type, pmap) 		do{ 									\
											if((type == 1 || type == 6))		\
											{									\
												RCC->APB2ENR |= (1U<<pmap);		\
											}									\
											else								\
											{									\
												RCC->APB1ENR |= (1U<<pmap);		\
											}									\
										} while(0)


/*
 * Clock Enable Macros for SYSCFG peripheral 
 */
#define SYSCGF_PCLK_EN()		(RCC->APB2ENR |= (1U<<SYSCFG))


/**
 * CLock Disable Macros for SPIx peripheral
 */







/**
 * Macros to reset GPIOx peripheral
 */
#define GPIOx_REG_RESET(pmap)	do {											\
										(RCC->AHB1RSTR |= (1U << pmap));		\
										(RCC->AHB1RSTR &= ~(1U << pmap));		\
									} while(0)



/**
 * return port code for given GPIOx base address
 */









#include "Future_Project\DEV\IO\stm32f40xx_gpio_driver.h"
#include "Future_Project\DEV\I2C\stm32f40xx_i2c_driver.h"
#include "Future_Project\DEV\SPI\stm32f40xx_SPI_driver.h"



#endif


