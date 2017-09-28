/////////////////////////////////////////////////////////////////////////
//
//										 SYS_Clock.c
//										(SYS_Clock.h)
//		Настройка системы тактирования.
//		Для МК: STM32F205, STM32F207, STM32F215, STM32F217
//
//		Foton6
//		29.10.2015
//
//
/////////////////////////////////////////////////////////////////////////


#include "SYS_Clock.h"


#ifdef __SYS_Clock

//______Битовые маски для работы с регитсрами RCC______//

#define cm_CFGR_DIV 			 0xFFC0C00F				//__Битовая маска настройки делителей APB1, APB2, AHB (регистр RCC_CFGR).
#define cm_ACR_LATENCY		 0xFFFFFFF8				//__Битовая маска настройки FLASH_ACR_LATENCY.

//______Настройка системы тактирования______//


	 uint32_t INIT_SYS_Clock (void)
	 {	
		 uint32_t i = 0;	
				 
		 //____________________Вклчение HSE_______________________//
		 
		#if d_CR_HSEON != 0
			RCC->CR |= RCC_CR_HSEON;			//__HSE - вкл.
			#if d_CR_HSEBYP != 0
				RCC->CR |= RCC_CR_HSEBYP;		//__Внешний ГЕНЕРАТОР - вкл.
			#endif
			i = 0;
			while(!(RCC->CR & RCC_CR_HSERDY))
			{
				i++;
				if(i >= 400)
				{	return (Error_1);
				}
			}
		#endif
				
		//_____________Настройка делителей APBx и AHB______________//	
	
		#if (d_CFGR_PPRE2 + d_CFGR_PPRE1 + d_CFGR_HPRE) != 0
				
									//_____Делитель APB2
			#if	d_CFGR_PPRE2 == 0
					#define d_PPRE2  0x00
			#elif d_CFGR_PPRE2 == 2
					#define d_PPRE2  0x04
			#elif d_CFGR_PPRE2 == 4
					#define d_PPRE2  0x05
			#elif d_CFGR_PPRE2 == 8
					#define d_PPRE2  0x06
			#elif d_CFGR_PPRE2 == 16
					#define d_PPRE2  0x07
			#else
					#error SYS_Clock.h - Incorrect value of "d_CFGR_PPRE2".
			#endif
			
									//_____Делитель APB1
			#if	d_CFGR_PPRE1 == 0
					#define d_PPRE1  0x00
			#elif d_CFGR_PPRE1 == 2
					#define d_PPRE1  0x04
			#elif d_CFGR_PPRE1 == 4
					#define d_PPRE1  0x05
			#elif d_CFGR_PPRE1 == 8
					#define d_PPRE1  0x06
			#elif d_CFGR_PPRE1 == 16
					#define d_PPRE1  0x07
			#else
					#error SYS_Clock.h - Incorrect value of "d_CFGR_PPRE1".
			#endif
			
									//_____Делитель AHB
			#if	d_CFGR_HPRE == 0
				#define d_HPRE  0x00
			#elif d_CFGR_HPRE == 2
				#define d_HPRE  0x08
			#elif d_CFGR_HPRE == 4
				#define d_HPRE  0x09
			#elif d_CFGR_HPRE == 8
				#define d_HPRE  0x0A
			#elif d_CFGR_HPRE == 16
				#define d_HPRE  0x0B
			#elif d_CFGR_HPRE == 64
				#define d_HPRE  0x0C
			#elif d_CFGR_HPRE == 128
				#define d_HPRE  0x0D
			#elif d_CFGR_HPRE == 256
				#define d_HPRE  0x0E
			#elif d_CFGR_HPRE == 512
				#define d_HPRE  0x0F
			#else
				#error SYS_Clock.h - Incorrect value of "d_CFGR_HPRE".
			#endif		
				RCC->CFGR = ((RCC->CFGR & cm_CFGR_DIV)|(d_PPRE2 << 11)|(d_PPRE1 << 8)|(d_HPRE << 4)|(d_PLLCFGR_PLLXTPRE << 17));
		#endif
		
		
		//____________________Настройка PLL_______________________//
		
		#if d_PLLCFGR_PLLMUL == 0
			#define d_PLLMUL  0
		#elif	d_PLLCFGR_PLLMUL == 4
			#define d_PLLMUL  0x02
		#elif	d_PLLCFGR_PLLMUL == 5
			#define d_PLLMUL  0x03
		#elif	d_PLLCFGR_PLLMUL == 6
			#define d_PLLMUL  0x04
		#elif	d_PLLCFGR_PLLMUL == 7
			#define d_PLLMUL  0x05
		#elif	d_PLLCFGR_PLLMUL == 8
			#define d_PLLMUL  0x06
		#elif	d_PLLCFGR_PLLMUL == 9
			#define d_PLLMUL  0x07
		#elif	d_PLLCFGR_PLLMUL == 6.5
			#define d_PLLMUL  0x0D
		#else
			#error SYS_Clock.h - Incorrect value of "d_PLLCFGR_PLLMUL".
		#endif
		
		RCC->CFGR |= ((d_PLLMUL << 18)| (d_PLLCFGR_PLLSRC << 16));
		RCC->CR |= RCC_CR_PLLON;						//__PLL - on (Включать только после настройки PLL).
		i = 0;
		while(!(RCC->CR & RCC_CR_PLLRDY))		//__Ожидание готовности PLL.
		{
			i++;
			if(i >= 100)
			{	return (Error_4);
			}
		}		
		
		//_______________Настройка FLASH_ACR_LATENCY_________________//	
			
		#if	d_FLASH_ACR_LATENCY != 0
			FLASH->ACR = ((FLASH->ACR & cm_ACR_LATENCY)|(d_FLASH_ACR_LATENCY)); 
		#endif
		
		//_______Выбор основного источника тактирования (SW)_________//

		#if d_CFGR_SW != 0	
			RCC->CFGR |= d_CFGR_SW;
		#endif
		
		for(i = 0; i < 999999; i++)
		{
		}		
		
		return (CLOCK_OK);
	}

#endif

