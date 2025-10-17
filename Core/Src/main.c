#include "stm32f1xx_hal.h"
#include "sysclock.h"

uint32_t  HCLKFreq;
uint32_t  PCLK1Freq;
uint32_t  PCLK2Freq;

int main(void){
	HAL_Init();
	RccClock_Init_HSE();
	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_SYSCLK,RCC_MCODIV_1);	
	SystemCoreClockUpdate();
	HCLKFreq = HAL_RCC_GetHCLKFreq();
	PCLK1Freq = HAL_RCC_GetPCLK1Freq();
	PCLK2Freq = HAL_RCC_GetPCLK2Freq();

}
