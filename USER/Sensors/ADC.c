

/* Includes ------------------------------------------------------------------*/

#include "Console.h"

void ADC_Configuration(int chan);
vu16 ADC_value;

/*******************************************************************************
* Function Name  : ADC_Configuration
* Description    : Configure the ADC.
*******************************************************************************/
void ADC_HAL(int chan)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(GPIOA, &GPIO_InitStructure);   
}
/*******************************************************************************
* Function Name  : ADC_Configuration
* Description    : Configure the ADC.
*******************************************************************************/
u16 Convert_ADC(u8 chan)
{
    	ADC_HAL(1);
			ADC_Configuration(12); 	  
	    return ADC_value;
}
/*******************************************************************************
* Function Name  : ADC_Configuration
* Description    : Configure the ADC.
*******************************************************************************/
void ADC_Configuration(int chan)
{
				ADC_InitTypeDef ADC_InitStructure;
				DMA_InitTypeDef DMA_InitStructure;
				RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

				/* DMA channel1 configuration ----------------------------------------------*/
				DMA_DeInit(DMA1_Channel1);
				DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_Address;
				DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_value;
				DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
				DMA_InitStructure.DMA_BufferSize = 1;
				DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
				DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
				DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
				DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
				DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
				DMA_InitStructure.DMA_Priority = DMA_Priority_High;
				DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
				DMA_Init(DMA1_Channel1, &DMA_InitStructure); 
				DMA_Cmd(DMA1_Channel1, ENABLE);   
				/* ADC1 configuration ------------------------------------------------------*/
				ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
				ADC_InitStructure.ADC_ScanConvMode = ENABLE;
				ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
				ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
				ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
				ADC_InitStructure.ADC_NbrOfChannel = 1;
				ADC_Init(ADC1, &ADC_InitStructure);
				ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
				ADC_DMACmd(ADC1, ENABLE);
				ADC_Cmd(ADC1, ENABLE);
				ADC_ResetCalibration(ADC1);
				while(ADC_GetResetCalibrationStatus(ADC1));
				ADC_StartCalibration(ADC1);
				while(ADC_GetCalibrationStatus(ADC1));
				ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
/*******************************************************************************
ADC_Channel_0: PA0
ADC_Channel_1: PA1
ADC_Channel_2: PA2
ADC_Channel_3: PA3
ADC_Channel_4: PA4
ADC_Channel_5: PA5
ADC_Channel_6: PA6
ADC_Channel_7: PA7
ADC_Channel_8: PB0
ADC_Channel_9: PB1
ADC_Channel_10: PC0
ADC_Channel_11: PC1
ADC_Channel_12: PC2
ADC_Channel_13: PC3
ADC_Channel_14: PC4
ADC_Channel_15: PC5
*******************************************************************************/

