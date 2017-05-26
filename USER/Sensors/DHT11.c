
#include "DHT11.h"

u8 DHT11_DATA[5];

void DHT11_IN_Init(void)
{
			GPIO_InitTypeDef GPIO_InitStructure;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
}

static void DHT11_OUT_Init(void)
{
			GPIO_InitTypeDef GPIO_InitStructure;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
}

static void DHT11_Rst(void)
{
			DHT11_OUT_Init();
			PAout(OUT_PIN_NUM) = 0;
			Delay_ms(19);
			PAout(OUT_PIN_NUM) = 1;
			Delay_us(30);
			DHT11_IN_Init();
}

static u8 DHT11_Check(void)
{
	return (u8)PAin(OUT_PIN_NUM);
}


static u8 DHT11_Read_Bit(void)
{
		while(!PAin(OUT_PIN_NUM));

		Delay_us(40);

		if (PAin(OUT_PIN_NUM) == 1)
		{
				while(PAin(OUT_PIN_NUM));
				return 1;
		}
		return 0;
}
u8 DHT11_Read_Byte(void)
{
		int i;
		u8 Data = 0;
		for (i = 0; i < 8; i++)
		{
				Data <<= 1;
				Data |= DHT11_Read_Bit();
		}
		return Data;
}

u8 Convert_DHT11(void)
{
		u8 i = 0;
		DHT11_Rst();
		if (DHT11_Check() == 0)
		{
			while (!PAin(OUT_PIN_NUM));
			while(PAin(OUT_PIN_NUM));
			for (i = 0; i< 5; i++)
			{
				 DHT11_DATA[i] = DHT11_Read_Byte();
			}
			while(!PAin(OUT_PIN_NUM));
			DHT11_OUT_Init();
			PAout(OUT_PIN_NUM) = 1;
			if ((DHT11_DATA[0] + DHT11_DATA[1] + DHT11_DATA[2] + DHT11_DATA[3]) ==DHT11_DATA[4])
			{
				 return BACK_SUCCESS;
			}
		}
		return BACK_ERROR;
}

