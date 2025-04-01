

#include <stm32f40xx.h>

#ifndef _STM32F40XX_SPI_H_
#define _STM32F40XX_SPI_H_

/**
 * Configuration structure for SPix peripheral 
 */
typedef struct 
{
    U08 SPI_DeviceMode;
    U08 SPI_BusConfig;
    U08 SPI_SCLKSpeed;
    U08 SPI_DFF;
    U08 SPI_CPOL;
    U08 SPI_CPHA;
    U08 SPI_SSM;
}SPI_Config_tp;


/**
 * Handle structure for SPIx peripheral
 */
typedef struct 
{
    SPI_TypeDef*    pSPIx;
    SPI_Config_tp   SPIConf;
    U08             *pTxBuffer;
    U08             *pRxBuffer;
    U32             TxLen;
    U32             RxLen;
    U08             TxState;
    U08             RxState;
}SPI_Handle_tp;


/**
 * @SPI_DeviceMode
 */
#define SPI_DEVICE_MODE_MASTER		1
#define SPI_DEVICE_MODE_SLAVE		0

#define SPI_BUS_CFG_FD              1
#define SPI_BUS_CFG_HD              2
#define SPI_BUS_CFG_SP_RXONLY       3

#define SPI_SCLK_SPEED_DIV2         0
#define SPI_SCLK_SPEED_DIV4         1
#define SPI_SCLK_SPEED_DIV8         2
#define SPI_SCLK_SPEED_DIV16        3
#define SPI_SCLK_SPEED_DIV32        4
#define SPI_SCLK_SPEED_DIV64        5
#define SPI_SCLK_SPEED_DIV128       6
#define SPI_SCLK_SPEED_DIV256       7

#define SPI_DFF_8BITS               0
#define SPI_DFF_16BITS              1

#define SPI_CPOL_HIGH               1
#define SPI_CPOL_LOW                0

#define SPI_CPHA_HIGH               1
#define SPI_CPHA_LOW                0

#define SPI_SSM_HW                  0
#define SPI_SSM_SW                  1

//SPI application states
#define SPI_READY                   0
#define SPI_BUSY_IN_RX              1
#define SPI_BUSY_IN_TX              2

//Possible SPI Application events
#define SPI_EVENT_TX_CMPL           1
#define SPI_EVENT_RX_CMPL           2
#define SPI_EVENT_OVR_ERR           3
#define SPI_EVENT_CRC_ERR           3

/********************************************************************************* */
/**
 * CR1 register map order
 */
enum
{
    CR1_CPHA,
    CR1_CPOL,
    CR1_MSTR,
    CR1_BR,
    CR1_SPE = 6,
    CR1_LSB_FIRST,
    CR1_SSI,
    CR1_SSM,
    CR1_RX_ONLY,
    CR1_DFF,
    CR1_CRC_NEXT,
    CR1_CRC_EN,
    CR1_BIDI_OE,
    CR1_BIDI_MODE
};

/**
 * CR2 register map order
 */
enum
{
    CR2_RXDMAEN,
    CR2_TXDMAEN,
    CR2_SSOE,
    CR2_FRF = 4,
    CR2_ERRIE,
    CR2_RXNEIE,
    CR2_TXEIE,
};

/**
 * SR register map order
 */
enum
{
    SR_RXNE,
    SR_TXE,
    SR_CHSIDE,
    SR_UDR,
    SR_CRC_ERR,
    SR_MODF,
    SR_OVR,
    SR_BSY,
    SR_FRE,
};

/****************************************************************************************** */
#define SPI_FLAG_TXE                (1 << SR_TXE)
#define SPI_FLAG_RXNE               (1 << SR_RXNE)
#define SPI_FLAG_BUSY               (1 << SR_BSY)




/*******************************************************************************
 *                         APIs supported by this driver
 * ******************************************************************************/

 /**
  * Peripheral clock setup
  */
void SPIx_PeriClockControl(U08 SPIx, U08 EnorDis);
void SPI_SSIConfig(SPI_TypeDef *pSPIx, U08 EnOrDi);

/**
 * Init and De-init
 */
void SPIx_init(U08 SPIx, SPI_Handle_tp* pSPIHandle);
void SPIx_DeInit(U08 SPIx);

/**
 * Data Send and Receive
 */
void SPI_DataTransfer(SPI_TypeDef* pSPIx, U08 *pTxBuffer, U32 len);
void SPI_DataReceive(SPI_TypeDef* pSPIx, U08 *pRxBuffer, U32 len);

U08 SPI_DataTransferIT(SPI_Handle_tp* pSPIHandle, U08 *pTxBuffer, U32 len);
U08 SPI_DataReceiveIT(SPI_Handle_tp* pSPIHandle, U08 *pRxBuffer, U32 len);

/**
 * IRQ Configuration and ISR handling
 */
void SPI_IRQIRConfig(U08 IRQNumber, U08 EnorDis);
void SPI_IRQPriorityConfig(U08 IRQNumber, U08 IRQPriority);
void SPI_IRQHandling(SPI_Handle_tp* pSPIHandle);

U08 SPI_GetFlagStatus(SPI_TypeDef* pSPIx, U32 FlagName);

void SPI_PeripheralControl(SPI_TypeDef* pSPIx, U08 EnorDis);
void SPI_ClearOVRFlag(SPI_TypeDef* pSPIx);
void SPI_CloseTransmission(SPI_Handle_tp* pSPIHandle);
void SPI_CloseReception(SPI_Handle_tp* pSPIHandle);

/**
 * Application callback
 */
void SPI_ApplicationEventCallBack(SPI_Handle_tp* pSPIHandle, U08 AppEv);
#endif
