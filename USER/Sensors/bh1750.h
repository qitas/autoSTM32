#ifndef __BH1750_H__
#define __BH1750_H__

#include "MAP.h"

#define BH1750_Addr 0x46
#define BH1750_ON   0x01
#define BH1750_CON  0x10
#define BH1750_ONE  0x20
#define BH1750_RSET 0x07

void Init_BH1750(void);				
void Start_BH1750(void);			
u16 Read_BH1750(void);        
u16 Convert_BH1750(void);

#endif

