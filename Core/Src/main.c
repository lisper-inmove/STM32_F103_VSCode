#include "stm32f1xx_hal.h"
#include "main.h"
#include "rcc.h"
#include "led.h"
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
	SW_Init();

	while (1) {
		switch(SW_Scan_DOWN()) {
			case 8: 
				HAL_GPIO_TogglePin(LED_GPIO_GROUP, LED_PIN);
				break;
			default: break;
		}
	}
}