
#include "i2c.h"
#include "Systime.h"

#define	IIC_Port 	GPIOB
#define	SCLK_Pin 	GPIO_Pin_8
#define	SDA_Pin   GPIO_Pin_9

#define SCL_H   GPIO_SetBits(IIC_Port, SCLK_Pin)
#define SCL_L   GPIO_ResetBits(IIC_Port, SCLK_Pin)
#define SDA_H   GPIO_SetBits(IIC_Port, SDA_Pin)
#define SDA_L   GPIO_ResetBits(IIC_Port, SDA_Pin)
#define SDA_I   GPIO_ReadInputDataBit(IIC_Port, SDA_Pin)

void i2c_Port_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	 
	GPIO_InitStructure.GPIO_Pin = SCLK_Pin|SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_Port, &GPIO_InitStructure);
	SCL_H;
	SDA_H;
}

void SDA_OUT(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = SDA_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
		GPIO_Init(IIC_Port,&GPIO_InitStructure);
}
void SDA_IN(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = SDA_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(IIC_Port,&GPIO_InitStructure);
}

void I2C_Start(void)
{
	SDA_OUT();  
	SDA_H;	  	  
	SCL_H;
	Delay_us(4);
 	SDA_L;
	Delay_us(4);
	SCL_L;
}	  

void I2C_Stop(void)
{
	SDA_OUT();
	SCL_L;
	SDA_L;
 	Delay_us(4);
	SCL_H; 
	SDA_H;
	Delay_us(4);							   	
}

u8 I2C_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_H;Delay_us(1);	   
	SCL_H;Delay_us(1);
	SDA_IN();   
	while(SDA_I)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	SCL_L;	   
	return 0;  
} 

void I2C_Ack(void)
{
	SCL_L;
	SDA_OUT();
	SDA_L;
	Delay_us(2);
	SCL_H;
	Delay_us(2);
	SCL_L;
}
	    
void I2C_NAck(void)
{
	SCL_L;
	SDA_OUT();
	SDA_H;
	Delay_us(2);
	SCL_H;
	Delay_us(2);
	SCL_L;
}					 				     
	  
void I2C_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    SCL_L;
    for(t=0;t<8;t++)
    {              
        if(txd&0x80) SDA_H;
			  else SDA_L;
        txd<<=1; 	  
		Delay_us(2);  
		SCL_H;
		Delay_us(2); 
		SCL_L;	
		Delay_us(2);
    }	 
} 	    

u8 I2C_Read_Byte(u8 ack)
{
	u8 i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ )
	{
        SCL_L; 
        Delay_us(2);
				SCL_H;
        receive<<=1;
        if(SDA_I)receive++;   
		    Delay_us(1); 
    }					 
    if (!ack)
        I2C_NAck();
    else
        I2C_Ack();    
    return receive;
}


