
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
	OLED_Init(0x78);			//__Инициализация OLED.
	I2C2_Struct = &OLED;	//__Настройка I2C на работу с OLED.

	I2C_IntEvEn();				//__Разрешить прерывания I2C.

	__enable_fiq();	//__Глобальное разрешение прерываний.

	OLED_SendSym(0, 0, &Barash, 0);

	while(1)
	{
		while(d_I2C_WaitBusy);
		d_I2C_Start();		//__Старт.
	}
}

////////////////////////////////////////////////////////////////////////






