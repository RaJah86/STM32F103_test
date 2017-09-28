
#include <stdint.h>
#include "stm32f10x.h"

#include "SYS_Clock.h"
#include "I2C.h"
#include "OLED_Mono_128x64.h"

////////////////////////////////////////////////////////////////////////

#include "bb.h"
SymbolType Barash = {64, 64, (uint8_t*)&BarashBuff[0]};	
	
////////////////////////////////////////////////////////////////////////

void SystemInit (void)
{
	uint32_t ret = 0;
	ret = INIT_SYS_Clock();		//__Настройка системы тактирования.
	if(100 < ret)
	{
		while(1);
	}

	////////////////////////////////////////////////////
	
	I2C_HeaderInit();			//__Инициализация I2C.
		
	////////////////////////////////////////////////////
/*	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;				//__Разрешить тактирование порта С.
	GPIOC->CRH |= GPIO_CRH_MODE13;						//__C13 как выход (50MHz).
	GPIOC->CRH &= ~GPIO_CRH_CNF13_0;					//
*/	
/*	
	GPIOC->CRL |= GPIO_CRL_MODE1;							//__C1 как выход (50MHz).
	GPIOC->CRL &= ~GPIO_CRL_CNF1_0;						//
*/
	////////////////////////////////////////////////////
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;		//__Разрешить тактирование порта С.
	
	GPIOC->CRL &= ~GPIO_CRL_CNF1_0;				//__С1 - вход с подтяжкой. 	
	GPIOC->CRL |=  GPIO_CRL_CNF1_1;				//	 
	GPIOC->CRL &= ~GPIO_CRL_MODE1;				//	
	GPIOC->ODR |= GPIO_ODR_ODR1;					//__Подтяжка к 1.
	
	GPIOC->CRH &= ~GPIO_CRH_CNF13_0;			//__С13 - вход с подтяжкой. 	
	GPIOC->CRH |=  GPIO_CRH_CNF13_1;			//	 
	GPIOC->CRH &= ~GPIO_CRH_MODE13;				//	
	GPIOC->ODR |= GPIO_ODR_ODR13;					//__Подтяжка к 1.	
	
	/////////////////////////////////////////////////////	
		
}

///////////////////////////////////////////////////////////////////////////////



int main (void)
{	
	uint32_t i, i2, i3 = 0;
	uint8_t i4 = 0;
	uint8_t X, Y;		
	OLED_Init(0x78);			//__Инициализация OLED.
	I2C2_Struct = &OLED;	//__Настройка I2C на работу с OLED.
	
	I2C_IntEvEn();				//__Разрешить прерывания I2C.
	
	
	__enable_fiq();	//__Глобальное разрешение прерываний.
			

	
	while(1)	
	{	
		OLED_SendSym(0, 0, &Barash, 0);

		for(i2 = 0; i2 < 15; i2++)
		{
			while(d_I2C_WaitBusy);
			if(i2)
			{
				X = (5 * (i2 - 1));
				Y = (3 * (i3 - 1));
			}
			//OLED_ClearXY(X, Y, 24, 20);
			X = (5 * i2);
			Y = (3 * i3);			
			//OLED_SendSym(X, Y, &Medusa, 0);
			//OLED_SendSym((X + Medusa.Width), Y, &Medusa, 1);	
			i3++;
			d_I2C_Start();		//__Старт.	
			for(i = 0; i < 399990; i++);		//__Тупо задержка времени.
		}
	}
}

////////////////////////////////////////////////////////////////////////






