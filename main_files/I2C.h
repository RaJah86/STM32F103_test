/////////////////////////////////////////////////////////////////////////
//
//										 I2C.h
//										(I2C.c)
//		Настройка I2C МК. 
//		Для МК: STM32F103
//
//		Foton6
//		11.04.2017
//
//
/////////////////////////////////////////////////////////////////////////

#ifndef __I2C_USE
#define	__I2C_USE

#include <stdint.h>
#include "stm32f10x.h"

#include "I2C.h"

#define d_StartEnd				(I2C2->SR1 & I2C_SR1_SB)		//__Старт сформирован?
#define d_AdrSendEnd			(I2C2->SR1 & I2C_SR1_ADDR)	//__Адрес отправлен?
#define d_ByteSendEnd			(I2C2->SR1 & I2C_SR1_TXE)		//__Данные отправлены?
#define d_I2C_WaitBusy		(I2C2->SR2 & I2C_SR2_BUSY)	//__Флаг занятости установлен?

#define d_I2C_Start()			I2C2->CR1 |= (I2C_CR1_START | I2C_CR1_PE)		//__Сформировать старт I2C2.
#define d_I2C_Stop()			I2C2->CR1 |= I2C_CR1_STOP		//__Сформировать стоп I2C2.

#define d_I2C_Byte(Byte)	I2C2->DR = Byte							//__Байт для отправки.

#define d_I2C_SR2_Clear()	(void) I2C2->SR2				//__Очищает регистр SR2 путем его чтения.

typedef struct
{
	uint8_t AdrSlave;
	uint16_t ByteNum;	
	uint8_t *AdrData;
	uint16_t DataLen;
	
}I2CStruct;

extern I2CStruct *I2C2_Struct;



void I2C_HeaderInit (void);		//__Инициализация I2C модулей(я), при помощи настроек
															//	заданных в заголовчном файле.


void I2C_IntEvEn (void);			//__Разрешение прерываний по событиям.




#endif		/*	__I2C_USE	*/
