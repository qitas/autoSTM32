#ifndef __i2c_H
#define __i2c_H

#include "MAP.h"

void i2c_Port_Init(void);    //初始化IIC的IO口				 
void I2C_Start(void);				//发送IIC开始信号 成功则返回1
void I2C_Stop(void);	  			//发送IIC停止信号
void I2C_Send_Byte(u8 txd);			//IIC发送一个字节
u8 I2C_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 I2C_Wait_Ack(void); 				//IIC等待ACK信号
void I2C_Ack(void);					//IIC发送ACK信号
void I2C_NAck(void);				//IIC不发送ACK信号
	  
#endif

