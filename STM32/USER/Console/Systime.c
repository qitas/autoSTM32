

/* Includes ------------------------------------------------------------------*/
#include "Systime.h"
/* Private variables ---------------------------------------------------------*/	 
static uint8_t  delay_fac_us = 0;  /* us延时倍乘数 */
static uint16_t delay_fac_ms = 0;  /* ms延时倍乘数 */
u16  TIME_MS=0;
u8 Systime_Mode=0;
/*******************************************************************************
* Function Name  :CLK_ON
* Description    :
*******************************************************************************/
u8 CLK_ON(u8 SYSCLK)
{	
	  RCC_ClocksTypeDef RCC_ClocksStatus;
    RCC_GetClocksFreq(&RCC_ClocksStatus);
	 switch(SYSCLK)
	 {
			case 72:
			{		
					RCC_DeInit(); 
				  RCC_HSEConfig(RCC_HSE_ON); 
					while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET); 
					RCC_HCLKConfig(RCC_SYSCLK_Div1);     
					RCC_PCLK1Config(RCC_HCLK_Div2);  
					RCC_PCLK2Config(RCC_HCLK_Div1);   
					RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  
					RCC_PLLCmd(ENABLE);
					while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);  
					RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  
					while(RCC_GetSYSCLKSource()!= 0x08); 
			}
			break;	
			case 64:
			{		
					RCC_DeInit(); 
					RCC_HSICmd(ENABLE);   
					while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); 
					RCC_HCLKConfig(RCC_SYSCLK_Div1);     
					RCC_PCLK1Config(RCC_HCLK_Div2);  
					RCC_PCLK2Config(RCC_HCLK_Div1);   
					RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_14);  
					RCC_PLLCmd(ENABLE);
					while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);  
					RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  
					while(RCC_GetSYSCLKSource()!= 0x08); 
			}
			break;
			case 2:
			{					 
			}
			break;	
		}
	  delay_fac_us = RCC_ClocksStatus.HCLK_Frequency /8000000;
		delay_fac_ms = RCC_ClocksStatus.HCLK_Frequency /8000;     
}

/*******************************************************************************
* Function Name  : delay_ms
* Description    :  
*******************************************************************************/   
void Delay_ms(u16 nms) 
{
	  u32 temp = delay_fac_ms * nms;
    if (temp > 0x00ffffff)
    {
        temp = 0x00ffffff;
    }
		SysTick->LOAD=temp;       
    SysTick->CTRL=0X01;        
    SysTick->VAL=0;   
		temp = 0;
		TIME_MS=nms;
	  while(!(temp>>16)) temp=SysTick->CTRL;   
		SysTick->CTRL=0;    
		SysTick->VAL=0;  
}
/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
*******************************************************************************/
void Delay(volatile s32 nCount)
{
    for(; nCount != 0; nCount--);
}

/*******************************************************************************
* Function Name  : Delay_us
* Description    : 
*******************************************************************************/            
void Delay_us(u16 Nus)
{
	  u32 temp=delay_fac_us*((Nus>1)?(Nus-1):0x5000)-2;
    SysTick->LOAD=temp;       
    SysTick->CTRL=1;        
    SysTick->VAL=0;     
    while(!(temp>>16)) temp=SysTick->CTRL; 	
    SysTick->CTRL=0;    
    SysTick->VAL=0;      
}


/*******************************************************************************
* Function Name  : delay_ms
* Description    :  
*******************************************************************************/   
u16 Wait_ms(u16 nms) 
{
	    Systime_Mode=1;
			SysTick->LOAD=delay_fac_ms;      
			SysTick->CTRL=0X03;        
			SysTick->VAL=0;   					
			TIME_MS=nms;		
      return 	TIME_MS;
}

void SysTick_Handler(void)
{
		if(Systime_Mode)
		{
			 if(TIME_MS!=0) TIME_MS--;
			 else 
			 {
				    SysTick->CTRL=0; 
					  Systime_Mode=0;
			 }
		}
}











