#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_pwr.h"
#include "sysclock.h"
#include "igpio.h"

int main(void){
	HAL_Init();
	RccClock_Init_HSE();
	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_SYSCLK,RCC_MCODIV_1);	
	SystemCoreClockUpdate();

	Led1_Init();
	SW1_Init_Evt();

	while(1) {
		for (int i = 0; i < 6; i++) {
			HAL_GPIO_TogglePin(Led1_GPIO_Group, Led1_GPIO_Pin);
			HAL_Delay(200);
		}
		HAL_GPIO_WritePin(Led1_GPIO_Group, Led1_GPIO_Pin, GPIO_PIN_SET);
		// 关时基中断
		HAL_SuspendTick();
		HAL_PWR_EnterSLEEPMode(0, PWR_SLEEPENTRY_WFE);
		// 开中断
		HAL_ResumeTick();
	}
}
