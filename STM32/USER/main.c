
#include "Console.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/*******************************************************************************
* Function Name  : main
* Description    : Main program
*******************************************************************************/
uint8_t color[11][3];
uint8_t Recv_data[5];

int main(void)
{
	  u8 cnt;
    u16 buff;
    Power_ON();	

	  while(1)
		{
			  buff=Convert_BH1750();
			  //Convert_DHT11();
			  Delay_ms(100);	
		}
		NVIC_SystemReset();
}



/*******************************************************************************
* Function Name  : IWDG_Configuration
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void IWDG_Configuration(void)
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); 
  IWDG_SetPrescaler(IWDG_Prescaler_256);         
  IWDG_SetReload(3905);			//25s				 
  IWDG_ReloadCounter();								
  IWDG_Enable(); 									
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
