
#include "BH1750.h"
#include "i2c.h"

u8 LX_BUF[2]={0};

void Init_BH1750(void)
{
	  i2c_Port_Init();
}

void Cmd_Write_BH1750(u8 cmd)
{
    I2C_Start();                
    I2C_Send_Byte(BH1750_Addr+0);  
	while(I2C_Wait_Ack());
    I2C_Send_Byte(cmd); 
	while(I2C_Wait_Ack());  
    I2C_Stop();                
	  Delay_ms(5);
}
void Start_BH1750(void)
{
	Cmd_Write_BH1750(BH1750_ON);	 //power on
	Cmd_Write_BH1750(BH1750_RSET);	//clear
	Cmd_Write_BH1750(BH1750_ONE);  //一次H分辨率模式，至少120ms，之后自动断电模式  
}
u16 Read_BH1750(void)
{ 
		u16 result;
		Start_BH1750();	
		Delay_ms(150);
		I2C_Start();                      
		I2C_Send_Byte(BH1750_Addr+1);       
		while(I2C_Wait_Ack());
		LX_BUF[0]=I2C_Read_Byte(1); 
		LX_BUF[1]=I2C_Read_Byte(0);  
    I2C_Stop();                      
    Delay_ms(5);
		result=LX_BUF[0];
		result=(result<<8)+LX_BUF[1]; 
	  return result;
}
u16 Convert_BH1750(void)
{
		u16 result_lx;
		Init_BH1750();
		result_lx=Read_BH1750();
		result_lx=(u16)result_lx/1.2;
    return result_lx;
}


