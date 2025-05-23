/*
 * stm32f40xx_i2c_driver.h
 *
 *  Created on: Apr 2, 2025
 *      Author: hatha
 */

#ifndef STM32F40XX_I2C_DRIVER_H_
#define STM32F40XX_I2C_DRIVER_H_

#include "Future_Project\HAL\25_chip_stm32\stm32f4xx.h"

/**
 * Configuration structure for user application
 */
typedef struct 
{
    U08 I2C_DeviceAddr;
    U08 I2C_SLCSpeed;
    U08 I2C_ACKControl;
    U08 I2C_FMDutyCycle;
}I2C_Conf_tp;

/**
 * Handle structure for I2Cx peripheral
 */
typedef struct 
{
    I2C_TypeDef*    pI2Cx;
    I2C_Conf_tp     I2CConf;
    U08             *pTxBuffer;
    U08             *pRxBuffer;
    U32             Txlen;
    U32             Rxlen;
    U08             TxRxState;
    U08             DevAddr;
    U32             RxSize;
    U08             Rs; /*<Repeat Start value>*/
}I2C_Handle_tp;


/************************* Enum for each register in peripheral **************************/
/**
 * I2C CR1 peripheral field
 */
enum
{
    I2C_CR1_PE,
    I2C_CR1_SMBUS,
    I2C_CR1_RESERVED1,
    I2C_CR1_SMBTYPE,
    I2C_CR1_ENARP,
    I2C_CR1_ENPEC,
    I2C_CR1_ENGC,
    I2C_CR1_NOSTRETCH,
    I2C_CR1_START,
    I2C_CR1_STOP,
    I2C_CR1_ACK,
    I2C_CR1_POS,
    I2C_CR1_PEC,
    I2C_CR1_ALERT,
    I2C_CR1_RESERVED2,
    I2C_CR1_SWRST,
};

/**
 * I2C CR2 peripheral field
 */
enum
{
    I2C_CR2_FREQ,
    I2C_CR2_ITERREN=8,
    I2C_CR2_ITEVTEN,
    I2C_CR2_ITBUFEN,
    I2C_CR2_DMAEN,
    I2C_CR2_LAST,
};

/**
 * I2C SR1 peripheral field
 */
enum
{
    I2C_SR1_SB,
    I2C_SR1_ADDR,
    I2C_SR1_BTF,
    I2C_SR1_ADD10,
    I2C_SR1_STOPF,
    I2C_SR1_Reserved,
    I2C_SR1_RxNE,
    I2C_SR1_TxE,
    I2C_SR1_BERR,
    I2C_SR1_ARLO,
    I2C_SR1_AF,
    I2C_SR1_OVR,
    I2C_SR1_PECERR,
    I2C_SR1_Reserved2,
    I2C_SR1_TIMEOUT,
    I2C_SR1_SMBALERT,
};

/**
 * I2C SR2 peripheral field
 */
enum
{
    I2C_SR2_PEC,
    I2C_SR2_DUALF,
    I2C_SR2_SMBHOST,
    I2C_SR2_SMBDEFAUL,
    I2C_SR2_GENCALL,
    I2C_SR2_Reserved,
    I2C_SR2_TRA,
    I2C_SR2_BUSY,
    I2C_SR2_MSL,
};
/************************************************************************************************************ */

/**
 * I2C_SPEED
 */
#define I2C_SCL_SPEED_SM        100000
#define I2C_SCL_SPEED_FM4K      400000
#define I2C_SCL_SPEED_FM2K      200000

/**
 * I2C_ACK CONTROL
 */
#define I2C_ACK_ENABLE          1
#define I2C_ACK_DISABLE         0

/**
 * I2C_FMDUTYCYLCE
 */
#define I2C_FM_DUTY_2           0
#define I2C_FM_DUTY_16_9        1

/**
 * I2C application status
 */
#define I2C_READY               0
#define I2C_BUSY_IN_RX          1 
#define I2C_BUSY_IN_TX          2
/**
 * 
 */
#define I2C_FLAG(field)            (1 << field)

#define I2C_DISABLE_RS          RESET
#define I2C_ENABLE_RS           SET

/**
 * I2C Application events macros
 */
#define I2C_EV_TX_CMPLT         0
#define I2C_EV_RX_CMPLT         1
#define I2C_EV_STOP             2
#define I2C_ERROR_BERR          3
#define I2C_ERROR_ARLO          4
#define I2C_ERROR_AF            5
#define I2C_ERROR_OVR           6
#define I2C_ERROR_TIMEOUT       7
#define I2C_EV_DATA_REQ         8
#define I2C_EV_DATA_RCV         9


/*******************************************************************************
 *                         APIs supported by this driver
 * ******************************************************************************/

/**
 * Peripheral clock setup
 */
void I2Cx_PeriClockControl(U08 I2Cx, U08 EnorDis);

/**
 * Init and De-init
 */
void I2Cx_init(U08 I2Cx, I2C_Handle_tp* pI2CHandle);
void I2Cx_DeInit(U08 I2Cx);

/**
 * Data Send and Receive
 */
void I2C_MasterSendData(I2C_Handle_tp *pI2CHandle, U08* pTxbuffer, U32 len, U08 SlaveAddr);
void I2C_MasterReceiveData(I2C_Handle_tp* pI2CHandle, U08 *pRxBuffer, U08 len, U08 SlaveAddr);

U08 I2C_MasterSendDataIT(I2C_Handle_tp *pI2CHandle, U08* pTxbuffer, U32 len, U08 SlaveAddr, U08 Rs);
U08 I2C_MasterReceiveDataIT(I2C_Handle_tp* pI2CHandle, U08 *pRxBuffer, U08 len, U08 SlaveAddr, U08 Rs);

void I2C_SlaveSendData(I2C_TypeDef *pI2Cx,U08 data);
U08 I2C_SlaveReceiveData(I2C_TypeDef *pI2Cx);
/**
 * Other API
 */
void I2C_ACKManage(I2C_TypeDef* pI2Cx, U08 EnorDis);
void I2C_MasterHanldeRXENInterrupt(I2C_Handle_tp* pI2CHandle);
void I2C_MasterHanldeTXEInterrupt(I2C_Handle_tp* pI2CHandle);
void I2C_CloseSendData(I2C_Handle_tp *pI2CHandler);
void I2C_CloseReceiveData(I2C_Handle_tp *pI2CHandler);

/**
 * IRQ Configuration and ISR handling
 */
void I2C_EV_IRQHandling(I2C_Handle_tp* pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_tp* pI2CHandle);

void I2C_IRQPriorityConfig(U08 IRQNumber,U32 IRQPriority);
void I2C_IRQInterruptConfig(U08 IRQNumber, U32 EnorDis);

U08 I2C_GetFlagStatus(I2C_TypeDef* pI2Cx, U32 FlagName);

/**
 * Application callback
 */
void I2C_ApplicationEventCallBack(I2C_Handle_tp* pI2CHandle, U08 AppEv);


#endif /* STM32F40XX_I2C_DRIVER_H_ */
