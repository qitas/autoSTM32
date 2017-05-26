

#ifndef MAP_H__
#define MAP_H__

#include "stm32f10x.h"

#define	  SN_ADDR  	0x0801E000
#define   ID_ADDR   0x0801E400 
#define   TR_ADDR   0x0801F000 

#define ADC1_Address    ((u32)0x4001244C)

void Delay_us(u16 i);
void Delay_ms(u16 i);
u16 Wait_ms(u16 nms);

typedef uint64_t    		u64;
typedef uint64_t 		    UINT64;

/*******************************************************************************
PA0-7 AIN     PB0-1 AIN     PA1 TIM2-CH2   PA8 TIM1-CH1   
PA11/12 CAN->IIC    
PB5 TIM3-CH2(IR_TX)   PB6/7 TIM4-CH1(IR_RX) PB8/9 TIM4-CH3/4 
PA9/10 USART1  PB10/11 USART3       
PB12-15 SPI2
PC13-15/PA15
*******************************************************************************/
#define BACK_SUCCESS       0   /*³É¹¦*/
#define BACK_ERROR         1   /*´íÎó*/
#define BACK_TIMEOUT       2   /*³¬Ê±*/


#define WORK_MODE      	0x11   
#define CHECK_MODE      0x12  
#define ADVER_MODE      0x13   

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef NULL
    #define NULL 0
#endif

#ifndef MSB
    #define MSB 0x80
#endif


#endif
