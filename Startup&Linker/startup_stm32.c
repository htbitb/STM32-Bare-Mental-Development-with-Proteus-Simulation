#include <stdint.h>

#define SRAM_STARTADDR      0x20000000UL
#define SRAM_SIZE           (128U * 1024) //128KB
#define SRAM_ENDADDR        (SRAM_STARTADDR + SRAM_SIZE)

#define STACK_START         SRAM_ENDADDR

extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

int main();

void Reset_Handler(void);

void NMI_Handler 					(void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler 				(void) __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler 				(void) __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler 				(void) __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler 			(void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler 					(void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler 				(void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler   				(void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler  				(void) __attribute__ ((weak, alias("Default_Handler")));
void WWDG_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));
void PVD_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));             
void TAMP_STAMP_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));      
void RTC_WKUP_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));                               
void RCC_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));             
void EXTI0_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));           
void EXTI1_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));           
void EXTI2_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));           
void EXTI3_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));           
void EXTI4_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));           
void DMA1_Stream0_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));    
void DMA1_Stream1_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));    
void DMA1_Stream2_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));    
void DMA1_Stream3_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));    
void DMA1_Stream4_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));    
void DMA1_Stream5_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));    
void DMA1_Stream6_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));    
void ADC_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));             
void CAN1_TX_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));         
void CAN1_RX0_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));        
void CAN1_RX1_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));        
void CAN1_SCE_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));        
void EXTI9_5_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));         
void TIM1_BRK_TIM9_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));   
void TIM1_UP_TIM10_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));   
void TIM1_TRG_COM_TIM11_IRQHandler 	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));         
void TIM2_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));            
void TIM3_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));            
void TIM4_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));            
void I2C1_EV_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));         
void I2C1_ER_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));         
void I2C2_EV_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));         
void I2C2_ER_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));         
void SPI1_IRQHandler  				(void) __attribute__ ((weak, alias("Default_Handler")));           
void SPI2_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));            
void USART1_IRQHandler  			(void) __attribute__ ((weak, alias("Default_Handler")));         
void USART2_IRQHandler  			(void) __attribute__ ((weak, alias("Default_Handler")));        
void USART3_IRQHandler   			(void) __attribute__ ((weak, alias("Default_Handler")));        
void EXTI15_10_IRQHandler   		(void) __attribute__ ((weak, alias("Default_Handler")));     
void RTC_Alarm_IRQHandler    		(void) __attribute__ ((weak, alias("Default_Handler")));    
void OTG_FS_WKUP_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler"))); 
void TIM8_BRK_TIM12_IRQHandler   	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_UP_TIM13_IRQHandler    	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_TRG_COM_TIM14_IRQHandler 	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_CC_IRQHandler          	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream7_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void FSMC_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void SDIO_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM5_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void SPI3_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void UART4_IRQHandler            	(void) __attribute__ ((weak, alias("Default_Handler")));
void UART5_IRQHandler            	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM6_DAC_IRQHandler         	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM7_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream0_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream1_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream2_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream3_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream4_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void ETH_IRQHandler              	(void) __attribute__ ((weak, alias("Default_Handler")));
void ETH_WKUP_IRQHandler         	(void) __attribute__ ((weak, alias("Default_Handler")));
void CAN2_TX_IRQHandler          	(void) __attribute__ ((weak, alias("Default_Handler")));
void CAN2_RX0_IRQHandler         	(void) __attribute__ ((weak, alias("Default_Handler")));
void CAN2_RX1_IRQHandler         	(void) __attribute__ ((weak, alias("Default_Handler")));
void CAN2_SCE_IRQHandler         	(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_FS_IRQHandler           	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream5_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream6_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream7_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void USART6_IRQHandler           	(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C3_EV_IRQHandler          	(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C3_ER_IRQHandler          	(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_HS_EP1_OUT_IRQHandler   	(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_HS_EP1_IN_IRQHandler    	(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_HS_WKUP_IRQHandler      	(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_HS_IRQHandler           	(void) __attribute__ ((weak, alias("Default_Handler")));
void DCMI_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void CRYP_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void HASH_RNG_IRQHandler         	(void) __attribute__ ((weak, alias("Default_Handler")));
void FPU_IRQHandler              	(void) __attribute__ ((weak, alias("Default_Handler")));

unint32_t vectors[] __attribute__((section(".isr_vector"))) = {
    STACK_START,
    (unint32_t)Reset_Handler,
    (unint32_t)NMI_Handler,
    (unint32_t)HardFault_Handler 				
    (unint32_t)MemManage_Handler, 				
    (unint32_t)BusFault_Handler, 				
    (unint32_t)UsageFault_Handler,
    0,
    0,
    0,
    0, 			
    (unint32_t)SVC_Handler, 					
    (unint32_t)DebugMon_Handler, 
    0,				
    (unint32_t)PendSV_Handler,   				
    (unint32_t)SysTick_Handler,  				
    (unint32_t)WWDG_IRQHandler, 				
    (unint32_t)PVD_IRQHandler, 				
    (unint32_t)TAMP_STAMP_IRQHandler, 			
    (unint32_t)RTC_WKUP_IRQHandler, 
    0,			
    (unint32_t)RCC_IRQHandler, 				
    (unint32_t)EXTI0_IRQHandler, 				
    (unint32_t)EXTI1_IRQHandler, 				
    (unint32_t)EXTI2_IRQHandler, 				
    (unint32_t)EXTI3_IRQHandler, 				
    (unint32_t)EXTI4_IRQHandler, 				
    (unint32_t)DMA1_Stream0_IRQHandler, 		
    (unint32_t)DMA1_Stream1_IRQHandler, 		
    (unint32_t)DMA1_Stream2_IRQHandler, 		
    (unint32_t)DMA1_Stream3_IRQHandler, 		
    (unint32_t)DMA1_Stream4_IRQHandler, 		
    (unint32_t)DMA1_Stream5_IRQHandler, 		
    (unint32_t)DMA1_Stream6_IRQHandler, 		
    (unint32_t)ADC_IRQHandler, 				
    (unint32_t)CAN1_TX_IRQHandler, 			
    (unint32_t)CAN1_RX0_IRQHandler, 			
    (unint32_t)CAN1_RX1_IRQHandler, 			
    (unint32_t)CAN1_SCE_IRQHandler, 			
    (unint32_t)EXTI9_5_IRQHandler, 			
    (unint32_t)TIM1_BRK_TIM9_IRQHandler, 		
    (unint32_t)TIM1_UP_TIM10_IRQHandler, 		
    (unint32_t)TIM1_TRG_COM_TIM11_IRQHandler, 	
    (unint32_t)TIM1_CC_IRQHandler, 			
    (unint32_t)TIM2_IRQHandler, 				
    (unint32_t)TIM3_IRQHandler, 				
    (unint32_t)TIM4_IRQHandler, 				
    (unint32_t)I2C1_EV_IRQHandler, 			
    (unint32_t)I2C1_ER_IRQHandler, 			
    (unint32_t)I2C2_EV_IRQHandler, 			
    (unint32_t)I2C2_ER_IRQHandler, 			
    (unint32_t)SPI1_IRQHandler,  				
    (unint32_t)SPI2_IRQHandler, 				
    (unint32_t)USART1_IRQHandler,  			
    (unint32_t)USART2_IRQHandler,  			
    (unint32_t)USART3_IRQHandler,   			
    (unint32_t)EXTI15_10_IRQHandler,   		
    (unint32_t)RTC_Alarm_IRQHandler,    		
    (unint32_t)OTG_FS_WKUP_IRQHandler,     	
    (unint32_t)TIM8_BRK_TIM12_IRQHandler,   	
    (unint32_t)TIM8_UP_TIM13_IRQHandler,    	
    (unint32_t)TIM8_TRG_COM_TIM14_IRQHandler, 	
    (unint32_t)TIM8_CC_IRQHandler,          	
    (unint32_t)DMA1_Stream7_IRQHandler,     	
    (unint32_t)FSMC_IRQHandler,             	
    (unint32_t)SDIO_IRQHandler,             	
    (unint32_t)TIM5_IRQHandler,             	
    (unint32_t)SPI3_IRQHandler,             	
    (unint32_t)UART4_IRQHandler,            	
    (unint32_t)UART5_IRQHandler,            	
    (unint32_t)TIM6_DAC_IRQHandler,         	
    (unint32_t)TIM7_IRQHandler,             	
    (unint32_t)DMA2_Stream0_IRQHandler,     	
    (unint32_t)DMA2_Stream1_IRQHandler,     	
    (unint32_t)DMA2_Stream2_IRQHandler,     	
    (unint32_t)DMA2_Stream3_IRQHandler,     	
    (unint32_t)DMA2_Stream4_IRQHandler,     	
    (unint32_t)ETH_IRQHandler,              	
    (unint32_t)ETH_WKUP_IRQHandler,         	
    (unint32_t)CAN2_TX_IRQHandler,          	
    (unint32_t)CAN2_RX0_IRQHandler,         	
    (unint32_t)CAN2_RX1_IRQHandler,         	
    (unint32_t)CAN2_SCE_IRQHandler,         	
    (unint32_t)OTG_FS_IRQHandler,           	
    (unint32_t)DMA2_Stream5_IRQHandler,     	
    (unint32_t)DMA2_Stream6_IRQHandler,     	
    (unint32_t)DMA2_Stream7_IRQHandler,     	
    (unint32_t)USART6_IRQHandler,           	
    (unint32_t)I2C3_EV_IRQHandler,          	
    (unint32_t)I2C3_ER_IRQHandler,          	
    (unint32_t)OTG_HS_EP1_OUT_IRQHandler,   	
    (unint32_t)OTG_HS_EP1_IN_IRQHandler,    	
    (unint32_t)OTG_HS_WKUP_IRQHandler,      	
    (unint32_t)OTG_HS_IRQHandler,           	
    (unint32_t)DCMI_IRQHandler,             	
    (unint32_t)CRYP_IRQHandler,             	
    (unint32_t)HASH_RNG_IRQHandler,         	
    (unint32_t)FPU_IRQHandler,              	
};


void Default_Handler(void)
{
    while(1);
}

void Reset_Handler(void)
{
    // Copy .data section to SRAM
    uint32_t size = &_edata - &_sdata;

    uint8_t *pDst = (uint8_t*)&_sdata; //sram
    uint8_t *pSrc = (uint8_t*)&_etext; //flash

    for(uint32_t i = 0; i < size; i++)
    {
        *pDst++ = *pSrc++;
    }
    // Init the .bss section to zero in SRAM

    size = &_ebss - &_sbss;
    pDst - (uint8_t*)&_sbss;
    for (uint32_t i = 0; i < 0; i++)
    {
        *pDst++ = 0;
    }

    // call main()
    main();

}