#include "iserial.h"
#include "igpio.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_uart.h"
#include <stdint.h>

UART_HandleTypeDef uart1;
UART_HandleTypeDef uart2;
UART_HandleTypeDef uart3;

uint8_t buf[DATA_BUF_SIZE];

void SerialInit(USART_TypeDef *usart, uint32_t baudRate, UART_HandleTypeDef *huart) {
    huart->Instance = usart;
    huart->Init.BaudRate = baudRate;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    HAL_UART_Init(huart);
}

void Serial_Loop(UART_HandleTypeDef *huart) {
    /**
        轮询方式收发数据
    */
    switch(HAL_UART_Receive(huart, buf, DATA_BUF_SIZE, TX_RX_TIME_OUT)) {
	case HAL_OK:
		HAL_Delay(1);
		HAL_UART_Transmit(huart, buf, DATA_BUF_SIZE, TX_RX_TIME_OUT);
		break;
	case HAL_TIMEOUT:
		uint32_t rCount = DATA_BUF_SIZE - huart->RxXferCount;
		if (rCount > 0) {
			HAL_UART_Transmit(huart, buf, rCount, TX_RX_TIME_OUT);
		} 
		break;
		default:
			break;
	}
}

void MspInit(UART_HandleTypeDef *huart, GPIO_TypeDef *gpioGroup, uint32_t txPin, uint32_t rxPin) {
    GPIO_InitTypeDef gpio;
    GPIO_CLK_ENABLE_BY_PORT(gpioGroup);
    USART_CLK_ENABLE_BY_PORT(huart->Instance);

    gpio.Pin = txPin;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(gpioGroup, &gpio);

    gpio.Pin = rxPin;
    gpio.Mode = GPIO_MODE_AF_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(gpioGroup, &gpio);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    // 串口对应的引脚初始化
    if (huart->Instance == USART1) {
        MspInit(huart, SERIAL_1_GPIO_Group, SERIAL_1_GPIO_TX_Pin, SERIAL_1_GPIO_RX_Pin);
    } else if (huart->Instance == USART2) {
        MspInit(huart, SERIAL_2_GPIO_Group, SERIAL_2_GPIO_TX_Pin, SERIAL_2_GPIO_RX_Pin);
    } else if (huart->Instance == USART3) {
        MspInit(huart, SERIAL_3_GPIO_Group, SERIAL_3_GPIO_TX_Pin, SERIAL_3_GPIO_RX_Pin);
    }
}
