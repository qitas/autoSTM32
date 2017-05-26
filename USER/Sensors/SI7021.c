
#include "SI7021.h"
#include "i2c.h"

float result_temp=0;
float result_humi=0;

u8 BUF[2]={0};
u16 result=0;

u8 Sys_status=0;

void delay_ms(uint16_t i) 
{
	 uint16_t x=0;  
	 while(i--)
	 {
			x=8000;  
			while(x--);    
	 }
}

//≥ı ºªØ
void Init_SI7021(void)
{
	i2c_Port_Init();
}

void Cmd_Write_SI7021(u8 cmd)
{
    I2C_Start();                
    I2C_Send_Byte(SI7021_Addr+0); 
		if(I2C_Wait_Ack())
		{
			Sys_status=0x12;
		}
    I2C_Send_Byte(cmd);          
		if(I2C_Wait_Ack())
		{
			Sys_status=0x14;
		}
    //SI7021_SendByte(REG_data);  
    I2C_Stop();                 
		delay_ms(5);
}

void Read_SI7021(void)
{   	
    I2C_Start();                        
    I2C_Send_Byte(SI7021_Addr+1);       
		if(I2C_Wait_Ack())
		{
			Sys_status=0x16;
		}
		delay_ms(12);
		BUF[0]=I2C_Read_Byte(1); 
		BUF[1]=I2C_Read_Byte(0); 
    I2C_Stop();               
    delay_ms(5);
}

void Convert_Humi(void)
{
	Cmd_Write_SI7021(0xE5);
	Read_SI7021();
	result=BUF[0];
	result=(result<<8)+BUF[1];  
	result_humi=(result*125.0/65536) - 6;
}

void Convert_Temp(void)
{
	Cmd_Write_SI7021(0xE3);
	Read_SI7021();
	result=BUF[0];
	result=(result<<8)+BUF[1];  
	result_temp=(result*175.72/65536) - 46.85;
}

