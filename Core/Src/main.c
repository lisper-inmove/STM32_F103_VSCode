#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_uart.h"
#include "sysclock.h"
#include "iserial.h"
#include "itimer.h"

#include <stdint.h>

int main(void){
	HAL_Init();
	RccClock_Init_HSE();
	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_SYSCLK,RCC_MCODIV_1);	
	SystemCoreClockUpdate();

	Timer1_Init(1999, 35999, 2);
	Timer2_Init(1999, 35999);
	Timer3_Init(1999, 35999);
	Timer4_Init(1999, 35999);

	SerialInit(USART1, U1_BAUD_RATE, &uart1);
	u1_printf("Hello STM32\n");

	int32_t num1 = 0;
	int32_t num2 = 0;
	int32_t num3 = 0;
	int32_t num4 = 0;

	while(1) {
		if(__HAL_TIM_GET_FLAG(&tim1, TIM_FLAG_UPDATE)) {
			__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
			num1++;
			u1_printf("Timer1 times: %d\n", num1);
		}
		if(__HAL_TIM_GET_FLAG(&tim2, TIM_FLAG_UPDATE)) {
			__HAL_TIM_CLEAR_FLAG(&tim2, TIM_FLAG_UPDATE);
			num2++;
			u1_printf("Timer2 times: %d\n", num2);
		}
		if(__HAL_TIM_GET_FLAG(&tim3, TIM_FLAG_UPDATE)) {
			__HAL_TIM_CLEAR_FLAG(&tim3, TIM_FLAG_UPDATE);
			num3++;
			u1_printf("Timer3 times: %d\n", num3);
		}
		if(__HAL_TIM_GET_FLAG(&tim4, TIM_FLAG_UPDATE)) {
			__HAL_TIM_CLEAR_FLAG(&tim4, TIM_FLAG_UPDATE);
			num4++;
			u1_printf("Timer4 times: %d\n", num4);
		}
	}
	
}
