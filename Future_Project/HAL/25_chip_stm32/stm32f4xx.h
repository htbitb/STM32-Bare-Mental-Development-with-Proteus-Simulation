#include<macro.h>

#ifndef INC_STM32F40XX_H_
#define INC_STM32F40XX_H_

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
#define SPI1_BASE				(APB2PERIPH_BASE + 0x3000ul)
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


typedef struct
{
	__IO U32 CR;
	__IO U32 PLLCFG;
	__IO U32 CFGR;
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

/********************** GPIO enum for macros used ************************/
enum
{
	GPIO_A = 0,
	GPIO_B,
	GPIO_C,
	GPIO_D,
	GPIO_E,
	GPIO_H = 7,
};

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


/**
 * Clock Enable Macros for GPIOx peripherals
 */
#define GPIOx_PCLOCK_EN(pmap)	(RCC->AHB1ENR |= (1<<pmap))

/**
 * Clock enable Macros for I2Cx peripherals
 */
#define I2C1_PCLOCK_EN()	(RCC->APB1ENR |= (1<<21))

/**
 * Clock enable Macros for SPIx peripherals
 */
#define SPI1_PCLOCK_EN()	(RCC->APB2ENR |= (1<<12))
























#endif


