#ifndef __SI7021_H__
#define __SI7021_H__

#define SI7021_Addr 				0x80
#define	HUMI_HOLD_MASTER		0xE5
#define	TEMP_HOLD_MASTER		0xE3
#define	HUMI_NOHOLD_MASTER	0xF5
#define	TEMP_NOHOLD_MASTER	0xF3


void Init_SI7021(void);							
void Read_SI7021(void);       
void Convert_Temp(void);
void Convert_Humi(void);
void Cmd_Write_SI7021(unsigned char cmd);	

#endif

