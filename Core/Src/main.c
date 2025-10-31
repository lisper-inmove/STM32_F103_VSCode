#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_uart.h"
#include "sysclock.h"
#include "iserial.h"

#include <stdint.h>

int main(void){
	HAL_Init();
	RccClock_Init_HSE();
	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_SYSCLK,RCC_MCODIV_1);	
	SystemCoreClockUpdate();

	SerialInit(USART1, U1_BAUD_RATE, &uart1);

	while(1) {
	}
	
}
