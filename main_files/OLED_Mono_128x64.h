/////////////////////////////////////////////////////////////////////////
//
//										 OLED_Mono_128x64.h
//										(OLED_Mono_128x64.c)
//		Работа с монохромным OLED 128x64. 
//
//		Foton6
//		13.02.2017
//
/////////////////////////////////////////////////////////////////////////



#ifndef __OLED_Mono_128x64
#define	__OLED_Mono_128x64

#include <stdint.h>
#include "stm32f10x.h"

#include "OLED_Mono_128x64.h"

#include "I2C.h"


/////////////////////////////////////////////////////////////////////////////////

//____Тип данных SymbolType
//
//		Используется для работы с символами.
//
//		Width - ширина символа в пикселях
//		Height - высота символа в пикселях
//		*Symbol - указатель на массив байт, содержащий
//							графические данные символа.

typedef struct
{
	uint8_t Width;
	uint8_t Hight;	
	uint8_t	*Symbol;
}SymbolType;


////////////////////////////////////////////////////////////////////////////////


///////////////////////____Инициализация OLED____///////////////////////////////
//
//	Функция ррасчитана на работу без прерываний и ДМА!!!
//
//	Подходит не ковсем дисплеям, так как направление строк и столбцов 
//  может отличатся. Если вывод данных происходит не коррекно, то необходимо
//	поигратся с настройками дисплея, а в частности с ремапингом столбов и строк.
//
//	AdrSlave - адресс дисплея на шине I2C.
//
/////////////////////////////////////////////////////////////////////////////////
void OLED_Init	(uint8_t AdrSlave);

///////////////////////____Очистка OLED____/////////////////////////
//
//	Функция ррасчитана на работу без прерываний и ДМА!!!
//
/////////////////////////////////////////////////////////////////////
void OLED_Clear (void);

/////////////////////////____Очистка части OLED дисплея____/////////////////////////////
//
//	Функция ррасчитана на работу с прерываниями или ДМА!
//	Функция модифицирует буффер, который в последствии целиком выводится на дисплей.
//	Так же требует выдачи команды СТАРТ после выполнения, это позволяет
//	сначала провести несколько операций с буффером а затем сформировать старт.
//	
//	Координаты XY это координаты верхнего левого пикселя начала очистки:
//	X - по горизонтали адрессация (0 - 127)
//	Y - по вертикали адрессация	(0 - 63)
//	
//	Hight - высота очищаемого фрагмента
//	Width - ширина очищаемого фрагмента
//
///////////////////////////////////////////////////////////////////////////////////////
void OLED_ClearXY(uint8_t X, uint8_t Y, uint8_t Hight, uint8_t Width);

/////////////////////____Отправка байта OLED дисплею____////////////////////////////
//
//	Функция ррасчитана на работу без прерываний и ДМА!!!
//
//	data - данные для отправки.
//	mod -  0 - данные
//				 1 - команда
//
///////////////////////////////////////////////////////////////////////////////////
void OLED_SendByte (uint8_t data, uint8_t mod);

//////////////____Отправка последовательности байт OLED дисплею____/////////////////
//
//	Функция ррасчитана на работу без прерываний и ДМА!!!
//
//	data 		- указатель на массив байт для отправки
//	LenMsg 	- колличество байт для отправки
//	mod 		-  0 - данные
//						 1 - команды
//
///////////////////////////////////////////////////////////////////////////////////
void OLED_Send (uint8_t* Data, uint16_t LenMsg, uint8_t Mod);

///////////////////////____Отправка символа OLED дисплею____////////////////////////////
//
//	Функция ррасчитана на работу с прерываниями или ДМА!
//	Функция модифицирует буффер, который в последствии целиком выводится на дисплей.
//	Так же требует выдачи команды СТАРТ после выполнения, это позволяет
//	сначала провести несколько операций с буффером а затем сформировать старт.
//	
//	Координаты X,Y это координаты верхнего левого пикселя:
//	X - по горизонтали адрессация (0 - 127)
//	Y - по вертикали адрессация	(0 - 63)
//	
//	Data - указатель на структуру необходимого символа. Структура
//					типа SymbolType.
//
//	Mirror - позволяет зеркаллировать символ по вертикальной оси
//						0 - нормальный вывод
//						1 - зеркалирование
//		
///////////////////////////////////////////////////////////////////////////////////////
void OLED_SendSym (uint8_t X, uint8_t Y, SymbolType *Data, uint8_t Mirror);

/////////////////////////____Вывод данных в HEX формате____/////////////////////////////
//
//	Функция ррасчитана на работу с прерываниями или ДМА!
//	Функция модифицирует буффер, который в последствии целиком выводится на дисплей.
//	Так же требует выдачи команды СТАРТ после выполнения, это позволяет
//	сначала провести несколько операций с буффером а затем сформировать старт.
//	
//	Координаты XY это координаты верхнего левого пикселя строки:
//	X - по горизонтали адрессация (0 - 127)
//	Y - по вертикали адрессация	(0 - 63)
//	
//	Data - данные для вывода
//	Len - число отображаемых полубайт! Не более 8!
//
//	Пример вывода (ознакомится внимательно!):    
//			OLED_OutHex(0, 0, 0x1D5E, 2);
//
//	на дисплее в самом начале (вверху слева) появится:
//			0xE5
//
///////////////////////////////////////////////////////////////////////////////////////
void OLED_OutHex(uint8_t X, uint8_t Y, uint32_t Data, uint8_t Len);

/////////////////////////____Вывод данных в DEC формате____/////////////////////////////
//
//	///	####  ЖУТКИЙ КОЛХОЗ, аж ПЗДЦ!!!!! Переделать как будет время.
//
//	Функция ррасчитана на работу с прерываниями или ДМА!
//	Функция модифицирует буффер, который в последствии целиком выводится на дисплей.
//	Так же требует выдачи команды СТАРТ после выполнения, это позволяет
//	сначала провести несколько операций с буффером а затем сформировать старт.
//	
//	Координаты XY это координаты верхнего левого пикселя строки:
//	X - по горизонтали адрессация (0 - 127)
//	Y - по вертикали адрессация	(0 - 63)
//	
//	Data - данные для вывода
//	
///////////////////////////////////////////////////////////////////////////////////////
void OLED_OutDec(uint8_t X, uint8_t Y, uint32_t Data);

/////////////////////////____Вывод данных в BIN формате____/////////////////////////////
//
//	Функция ррасчитана на работу с прерываниями или ДМА!
//	Функция модифицирует буффер, который в последствии целиком выводится на дисплей.
//	Так же требует выдачи команды СТАРТ после выполнения, это позволяет
//	сначала провести несколько операций с буффером а затем сформировать старт.
//	
//	Координаты XY это координаты верхнего левого пикселя строки:
//	X - по горизонтали адрессация (0 - 127)
//	Y - по вертикали адрессация	(0 - 63)
//	
//	Data - данные для вывода
//	
///////////////////////////////////////////////////////////////////////////////////////
void OLED_OutBin(uint8_t X, uint8_t Y, uint8_t Data);



///////////////////////////////////////////////////////////////////////////////////////

//____Структура для работы с I2C
//
//		Содержит данные о выводимом буффере и адресс дисплея.
//
//		AdrSlave	= 0		(устанавливается при инициализации).
//		ByteNum		= ****	
//		*AdrData	= OLED_BuffOut
//		DataLen 	= 1024
//
//
extern I2CStruct OLED; 

			//////////////____Набор символов для вывода____////////////////
			
//		Для вывода симметричных символов содержащих только
//		половину изображения необходимо сделать
//		двы вызова OLED_SendSym(), один из вызовов
//		должен быть с опцией зеркалирования и поправкой координат.
//
//		Пример вывода медузы:
//
//		OLED_SendSym(0, 0, &Medusa, 0);
//		OLED_SendSym(0 + Medusa.Width, 0, &Medusa, 1);	
//


//____Медуза
//
//		Содержит половину симметричного символа Медузы.
//
extern SymbolType Medusa;


//____Текстовые символы
//
//		Содержит символы (в порядке расположения в массиве):
//
//		0...9, A...F, x, -, +
//
extern SymbolType HexSym[19];





#endif		/*	__OLED_Mono_128x64		*/



