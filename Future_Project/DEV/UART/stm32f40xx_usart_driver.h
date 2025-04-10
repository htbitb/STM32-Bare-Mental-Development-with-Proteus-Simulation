/*
 * stm32f40xx_usart_driver.h
 *
 *  Created on: Apr 9, 2025
 *      Author: hatha
 */

#ifndef STM32F40XX_USART_DRIVER_H_
#define STM32F40XX_USART_DRIVER_H_

#include <stm32f40xx.h>

/**
 * Configuration structure for USARTx peripheral
 */
typedef struct 
{
    U08 USART_Mode;
    U32 USART_Baud;
    U08 USART_NoOfStopBits;
    U08 USART_WordLength;
    U08 USART_ParityControl;
    U08 USART_HWFlowControl;   
}USART_Config;

/**
 * Handle structure for USARTx peripheral
 */
typedef struct 
{
    USART_TypeDef   *pUSARTx;
    USART_Config    USART_Conf;
    U08             *pTxBuffer;
	U08             *pRxBuffer;
	U32             TxLen;
	U32             RxLen;
	U08             TxBusyState;
	U08             RxBusyState;
}USART_Handler;

/*
 *Possible options for USART_Mode
 */
#define USART_MODE_ONLY_TX 	0
#define USART_MODE_ONLY_RX 	1
#define USART_MODE_TXRX  	2

/*
 *Possible options for USART_Baud
 */
#define USART_STD_BAUD_1200					1200
#define USART_STD_BAUD_2400					2400
#define USART_STD_BAUD_9600					9600
#define USART_STD_BAUD_19200 				19200
#define USART_STD_BAUD_38400 				38400
#define USART_STD_BAUD_57600 				57600
#define USART_STD_BAUD_115200 				115200
#define USART_STD_BAUD_230400 				230400
#define USART_STD_BAUD_460800 				460800
#define USART_STD_BAUD_921600 				921600
#define USART_STD_BAUD_2M 					2000000
#define SUART_STD_BAUD_3M 					3000000

/*
 *Possible options for USART_ParityControl
 */
#define USART_PARITY_EN_ODD   2
#define USART_PARITY_EN_EVEN  1
#define USART_PARITY_DISABLE   0

/*
 *Possible options for USART_WordLength
 */
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1

/*
 *Possible options for USART_NoOfStopBits
 */
#define USART_STOPBITS_1     0
#define USART_STOPBITS_0_5   1
#define USART_STOPBITS_2     2
#define USART_STOPBITS_1_5   3


/*
 *Possible options for USART_HWFlowControl
 */
#define USART_HW_FLOW_CTRL_NONE    	0
#define USART_HW_FLOW_CTRL_CTS    	1
#define USART_HW_FLOW_CTRL_RTS    	2
#define USART_HW_FLOW_CTRL_CTS_RTS	3

/*
 * USART flags
 */

#define USART_FLAG_TXE 			( 1 << USART_SR_TXE)
#define USART_FLAG_RXNE 		( 1 << USART_SR_RXNE)
#define USART_FLAG_TC 			( 1 << USART_SR_TC)

/*
* Application states
*/
#define USART_BUSY_IN_RX 1
#define USART_BUSY_IN_TX 2
#define USART_READY 0

/**
 * enum for SR register
 */
enum
{
    USART_SR_PE,
    USART_SR_FE,
    USART_SR_NF,
    USART_SR_ORE,
    USART_SR_IDLE,
    USART_SR_RXNE,
    USART_SR_TC,
    USART_SR_TXE,
    USART_SR_LBD,
    USART_SR_CTS,
};

/**
 * enum for CR1
 */
enum
{
    USART_CR1_SBK,
    USART_CR1_RWU,
    USART_CR1_RE,
    USART_CR1_TE,
    USART_CR1_IDLEIE,
    USART_CR1_RXNEIE,
    USART_CR1_TCIE,
    USART_CR1_TXEIE,
    USART_CR1_PEIE,
    USART_CR1_PS,
    USART_CR1_PCE,
    USART_CR1_WAKE,
    USART_CR1_M,
    USART_CR1_UE,
    USART_CR1_OVER8 = 15,
};

/**
 * enum for CR2
 */
enum
{
    USART_CR2_ADD = 0,
    USART_CR2_LBDL = 5,
    USART_CR2_LBDIE,
    USART_CR2_LBCL = 8,
    USART_CR2_CPHA,
    USART_CR2_CPOL,
    USART_CR2_CLKEN,
    USART_CR2_STOP,
    USART_CR2_LINEN = 14,
};

/**
 * enum for CR3
 */
enum
{
    USART_CR3_EIE,
    USART_CR3_IREN,
    USART_CR3_IRLP,
    USART_CR3_HDSEL,
    USART_CR3_NACK,
    USART_CR3_SCEN,
    USART_CR3_DMAR,
    USART_CR3_DMAT,
    USART_CR3_RTSE,
    USART_CR3_CTSE,
    USART_CR3_CTSIE,
    USART_CR3_ONEBIT,
};

/**
 * enum event and error handler
 */
enum
{
    USART_EVENT_TX_CMPLT,
    USART_EVENT_RX_CMPLT,
    USART_EVENT_IDLE,
    USART_EVENT_CTS,
    USART_EVENT_PE,
    USART_ERR_FE,
    USART_ERR_NE,
    USART_ERR_ORE,
};

/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/


/*
 * Peripheral Clock setup
 */
void USART_PeriClockControl(U08 USARTx, U08 EnOrDis);

/*
 * Init and De-init
 */
void USART_Init(U08 USARTx, USART_Handler *pUSARTHandle);
void USART_DeInit(USART_Handler *pUSARTHandle);

/*
 * Data Send and Receive
 */
void USART_SendData(USART_Handler *pUSARTHandle, U08 *pTxBuffer, U32 Len);
void  USART_ReceiveData(USART_Handler *pUSARTHandle,U08 *pRxBuffer, U32 Len);
U08 USART_SendDataIT(USART_Handler *pUSARTHandle,U08 *pTxBuffer, U32 Len);
U08 USART_ReceiveDataIT(USART_Handler *pUSARTHandle,U08 *pRxBuffer, U32 Len);

/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQInterruptConfig(U08 IRQNumber, U08 EnorDi);
void USART_IRQPriorityConfig(U08 IRQNumber, U32 IRQPriority);
void USART_IRQHandling(USART_Handler *pUSARTHandle);

/*
 * Other Peripheral Control APIs
 */

U08 USART_GetFlagStatus(USART_TypeDef *pUSARTx, U08 StatusFlagName);
void USART_ClearFlag(USART_TypeDef *pUSARTx, uint16_t StatusFlagName);
void USART_PeripheralControl(USART_TypeDef *pUSARTx, U08 EnOrDi);
void USART_SetBaudRate(USART_TypeDef *pUSARTx, U32 BaudRate, U08 USARTx);


/*
 * Application Callbacks
 */
void USART_ApplicationEventCallback(USART_Handler *pUSARTHandle,U08 ApEv);

#endif /* STM32F40XX_USART_DRIVER_H_ */
