#include "stm32f1xx_hal.h"
#include "sysclock.h"
#include "igpio.h"

int main(void){
	HAL_Init();
	RccClock_Init_HSE();
	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_SYSCLK,RCC_MCODIV_1);	
	SystemCoreClockUpdate();

	Led1_Init();
	SW1_Init();

	while(1) {
		SW1_Scan_Up_Delay();
	}
}
