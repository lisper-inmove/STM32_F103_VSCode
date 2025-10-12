#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_pwr.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_gpio.h"
#include "main.h"
#include "rcc.h"
#include "led.h"
#include "sw.h"
#include "itime.h"
#include "uart.h"

int main(void) {
	HAL_Init();
	RccClock_Init();
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
	SystemCoreClockUpdate();

	Timer1_Init(1999, 35999, 2);
	Timer2_Init(2999, 35999);
	Timer3_Init(3999, 35999);
	Timer4_Init(4999, 35999);
	UART_Init(UART1_BaudRate);

	uint8_t num1 = 0;
	uint8_t num2 = 0;
	uint8_t num3 = 0;
	uint8_t num4 = 0;
	while (1) {
		if (__HAL_TIM_GET_FLAG(&tim1, TIM_FLAG_UPDATE)) {
			__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
			num1++;
			uprintf("TIM1 Current value is %d\n", num1);
		}
		if (__HAL_TIM_GET_FLAG(&tim2, TIM_FLAG_UPDATE)) {
			__HAL_TIM_CLEAR_FLAG(&tim2, TIM_FLAG_UPDATE);
			num2++;
			uprintf("TIM2 Current value is %d\n", num2);
		}
		if (__HAL_TIM_GET_FLAG(&tim3, TIM_FLAG_UPDATE)) {
			__HAL_TIM_CLEAR_FLAG(&tim3, TIM_FLAG_UPDATE);
			num3++;
			uprintf("TIM3 Current value is %d\n", num3);
		}
		if (__HAL_TIM_GET_FLAG(&tim4, TIM_FLAG_UPDATE)) {
			__HAL_TIM_CLEAR_FLAG(&tim4, TIM_FLAG_UPDATE);
			num4++;
			uprintf("TIM4 Current value is %d\n", num4);
			if (num4 >= 5) {
				HAL_TIM_Base_Stop(&tim4);
				HAL_TIM_Base_DeInit(&tim4);
			}
		}
	}
}