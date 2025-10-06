#include "stm32f1xx_hal.h"
#include "main.h"
#include "rcc.h"
#include "led.h"
#include "stm32f1xx_hal_pwr.h"
#include "sw.h"
#include "stm32f1xx_hal_gpio.h"

int main(void) {
	HAL_Init();
	RccClock_Init();
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
	SystemCoreClockUpdate();
	uint32_t HCLKFreq = HAL_RCC_GetHCLKFreq();
	uint32_t PCLK1Freq = HAL_RCC_GetPCLK1Freq();
	uint32_t PCLK2Freq = HAL_RCC_GetPCLK2Freq();

	LED_Init();
	SW_Init_Event();

	while (1) {
		for (int i = 0; i < 10; i++) {
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			HAL_Delay(500);
		}
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

		// 关中断
		HAL_SuspendTick();
		// 进入睡眠模式
		HAL_PWR_EnterSLEEPMode(0, PWR_SLEEPENTRY_WFE);
		// 恢复中断
		HAL_ResumeTick();
	}
}