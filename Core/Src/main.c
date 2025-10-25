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

	// 5秒之前，可以通过按钮控制Led。5秒之后，按钮不能控制Led
	HAL_Delay(5000);
	// LockPin之后，DeInit不生效（5秒之后也能控制 Led）
	HAL_GPIO_LockPin(Led1_GPIO_Group, Led1_GPIO_Pin);
	HAL_GPIO_DeInit(Led1_GPIO_Group, Led1_GPIO_Pin);

	while(1) {
	}
}
