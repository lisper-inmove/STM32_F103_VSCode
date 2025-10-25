#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "sysclock.h"
#include "igpio.h"

int main(void){
	HAL_Init();
	RccClock_Init_HSE();
	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_SYSCLK,RCC_MCODIV_1);	
	SystemCoreClockUpdate();

	Led1_Init();
	SW1_Init_IT();

	while(1) {
		HAL_Delay(500);
		// 软件触发中断（软件触发按键按下时触发的事件），直接进入中断处理函数
		__HAL_GPIO_EXTI_GENERATE_SWIT(SW1_GPIO_Pin);
	}
}
