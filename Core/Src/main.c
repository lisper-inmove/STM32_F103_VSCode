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

#define RX_SIZE 200

uint8_t buff[256];

int main(void) {
	HAL_Init();
	RccClock_Init();
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
	SystemCoreClockUpdate();

	UART1_Init(921600);

	while (1) {
		switch(HAL_UART_Receive(&uart1, buff, RX_SIZE, 200)) {
			case HAL_OK:
				HAL_UART_Transmit(&uart1, buff, RX_SIZE, 200);
				break;
			case HAL_TIMEOUT:
				if (uart1.RxXferCount != (RX_SIZE - 1) && uart1.RxXferCount != (RX_SIZE)) {
					HAL_UART_Transmit(&uart1, buff, RX_SIZE - 1 - uart1.RxXferCount, 200);
				}
				break;
			default:
				break;
		}
	}
}