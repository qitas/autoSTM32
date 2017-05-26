
/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"   
#include "stm32f10x_iwdg.h" 
#include "stm32f10x_pwr.h" 
#include "stm32f10x_exti.h" 
#include "stm32f10x_adc.h" 
#include "Systime.h"
#include "RTC_Time.h" 
#include "bh1750.h"
#include "DHT11.h"


u8 Power_ON(void);
void Init(void);

u16 Convert_ADC(u8 chan);
void SERVO(u16 angle);

extern u8 Recv_Byte[16];
extern vu16 ADC_value;


