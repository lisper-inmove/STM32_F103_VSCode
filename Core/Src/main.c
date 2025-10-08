#include "stm32f1xx_hal.h"
#include "main.h"
#include "rcc.h"
#include "led.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_pwr.h"
#include "stm32f1xx_hal_uart.h"
#include "sw.h"
#include "uart.h"
#include "stm32f1xx_hal_gpio.h"

uint8_t buff[256];

int main(void) {
	HAL_Init();
	RccClock_Init();
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
	SystemCoreClockUpdate();

	UART_Init(UART1_BaudRate);

	while (1) {}
}