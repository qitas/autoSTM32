
#include "tlv.h"
#include "Console.h"

/*******************************************************************************
* Function Name  :Power_Init
* Description    :
*******************************************************************************/
u8 Power_ON(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;
	  CLK_ON(72);
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE); // |  
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
		GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/*******************************************************************************
* Function Name  :Pair_Mode
* Description    :
*******************************************************************************/
 
void Init(void)
{
		//Init_SI7021();
}