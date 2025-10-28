#include "iserial.h"
#include "igpio.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_uart.h"
#include <stdint.h>
#include <string.h>

UART_HandleTypeDef uart1;
UART_HandleTypeDef uart2;
UART_HandleTypeDef uart3;

uint8_t rxbuf[DATA_BUF_SIZE];
uint8_t txbuf[DATA_BUF_SIZE];

void SerialInit(USART_TypeDef *usart, uint32_t baudRate, UART_HandleTypeDef *huart) {
    huart->Instance = usart;
    huart->Init.BaudRate = baudRate;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    HAL_UART_Init(huart);

    /**
        HAL_UART_Init 会调用 HAL_UART_MspInit 函数
        最终会   huart->gState = HAL_UART_STATE_READY;
        在 HAL_UART_Receive_IT 中，在 huart->gState 为 HAL_UART_STATE_READY时，
        会调用 UART_Start_Receive_IT
    */
    #if SERIAL_1_Enable_IT || SERIAL_2_Enable_IT || SERIAL_3_Enable_IT
    HAL_UART_Receive_IT(huart, rxbuf, DATA_BUF_SIZE);
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    // __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
    #endif
}

void Serial_Loop(UART_HandleTypeDef *huart) {
    /**
        轮询方式收发数据
    */
    switch(HAL_UART_Receive(huart, rxbuf, DATA_BUF_SIZE, TX_RX_TIME_OUT)) {
	case HAL_OK:
		HAL_Delay(1);
		HAL_UART_Transmit(huart, rxbuf, DATA_BUF_SIZE, TX_RX_TIME_OUT);
		break;
	case HAL_TIMEOUT:
		uint32_t rCount = DATA_BUF_SIZE - huart->RxXferCount;
		if (rCount > 0) {
			HAL_UART_Transmit(huart, rxbuf, rCount, TX_RX_TIME_OUT);
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
        #if SERIAL_1_Enable_IT
        HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        #endif
    } else if (huart->Instance == USART2) {
        MspInit(huart, SERIAL_2_GPIO_Group, SERIAL_2_GPIO_TX_Pin, SERIAL_2_GPIO_RX_Pin);
        #if SERIAL_2_Enable_IT
        HAL_NVIC_SetPriority(USART2_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
        #endif
    } else if (huart->Instance == USART3) {
        MspInit(huart, SERIAL_3_GPIO_Group, SERIAL_3_GPIO_TX_Pin, SERIAL_3_GPIO_RX_Pin);
        #if SERIAL_3_Enable_IT
        HAL_NVIC_SetPriority(USART3_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
        #endif
    }
}

void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&uart1);

    if(__HAL_UART_GET_FLAG(&uart1, UART_FLAG_IDLE)){
		__HAL_UART_CLEAR_IDLEFLAG(&uart1);
        int32_t count = DATA_BUF_SIZE - uart1.RxXferCount;

        // Abort如果放在if语句的后面，会导致后面每次uart1.RxXferCount为0
		HAL_UART_AbortReceive_IT(&uart1);
        if (count > 0) {
            memcpy(txbuf, rxbuf, count);
            HAL_UART_Transmit_IT(&uart1, txbuf, count);
            // HAL_UART_Receive_IT中会把uart1.RxXferCount设置为Size
            // 如果在Receive之后再调用 AbortReceive，则会导致RxXferCount为0
	        HAL_UART_Receive_IT(&uart1, rxbuf, DATA_BUF_SIZE);
        }
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    /**
        接收数据完成时的回调函数 收发字节数要 DATA_BUF_SIZE 与一致
        这种方式很不灵活
    */
	if(huart->Instance == USART1) {
        memcpy(txbuf, rxbuf, DATA_BUF_SIZE);
        HAL_UART_Transmit_IT(&uart1, txbuf, DATA_BUF_SIZE);
	    HAL_UART_Receive_IT(&uart1, rxbuf, DATA_BUF_SIZE);
	} else if(huart->Instance == USART2) {
        memcpy(txbuf, rxbuf, DATA_BUF_SIZE);
        HAL_UART_Transmit_IT(&uart2, txbuf, DATA_BUF_SIZE);
	    HAL_UART_Receive_IT(&uart2, rxbuf, DATA_BUF_SIZE);
	} else if(huart->Instance == USART3) {
        memcpy(txbuf, rxbuf, DATA_BUF_SIZE);
        HAL_UART_Transmit_IT(&uart3, txbuf, DATA_BUF_SIZE);
	    HAL_UART_Receive_IT(&uart3, rxbuf, DATA_BUF_SIZE);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1) {

	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1) {

	}
}