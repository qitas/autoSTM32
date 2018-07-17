
#ifndef __SYSTIME_H
#define __SYSTIME_H 

#include "map.h"
extern u8 Systime_Mode;
extern u16  TIME_MS;

u8 CLK_ON(u8 SYSCLK);
void Delay_ms(u16 nms);
void Delay_us(u16 Nus);
u16 Wait_ms(u16 nms);
#endif






























