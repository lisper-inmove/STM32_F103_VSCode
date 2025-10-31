#include "iserial.h"
#include "igpio.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_uart.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

UART_HandleTypeDef uart1;

DMA_HandleTypeDef dmatx;
DMA_HandleTypeDef dmarx;

uint8_t rxbuf[DATA_BUF_SIZE];
uint8_t txbuf[DATA_BUF_SIZE];

void Led1_Init(void) {
    GPIO_InitTypeDef gpio;
    GPIO_CLK_ENABLE_BY_PORT(GPIOA);
    gpio.Pin = GPIO_PIN_5;
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &gpio);
}

void SerialInit(USART_TypeDef *usart, uint32_t baudRate, UART_HandleTypeDef *huart) {
    huart->Instance = usart;
    huart->Init.BaudRate = baudRate;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    HAL_UART_Init(huart);

    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(huart, rxbuf, DATA_BUF_SIZE);

    Led1_Init();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef gpio;
    if (huart->Instance == USART1) {

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

    gpio.Pin = GPIO_PIN_9;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &gpio);

    gpio.Pin = GPIO_PIN_10;
    gpio.Mode = GPIO_MODE_AF_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio);

    HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    dmatx.Instance = DMA1_Channel4;
    dmatx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    dmatx.Init.PeriphInc = DMA_PINC_DISABLE;
    dmatx.Init.MemInc = DMA_MINC_ENABLE;
    dmatx.Init.PeriphDataAlignment= DMA_PDATAALIGN_BYTE;
    dmatx.Init.MemDataAlignment= DMA_MDATAALIGN_BYTE;
    dmatx.Init.Mode = DMA_NORMAL;
    dmatx.Init.Priority = DMA_PRIORITY_MEDIUM;
    __HAL_LINKDMA(&uart1, hdmatx, dmatx);
    HAL_DMA_Init(&dmatx);

	HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

    dmarx.Instance = DMA1_Channel5;
    dmarx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dmarx.Init.PeriphInc = DMA_PINC_DISABLE;
    dmarx.Init.MemInc = DMA_MINC_ENABLE;
    dmarx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dmarx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dmarx.Init.Mode = DMA_NORMAL;
    dmarx.Init.Priority = DMA_PRIORITY_MEDIUM;
    __HAL_LINKDMA(&uart1, hdmarx, dmarx);
    HAL_DMA_Init(&dmarx);
    		
	HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    
    }
}

void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&uart1);

    if(__HAL_UART_GET_FLAG(&uart1, UART_FLAG_IDLE)){
		__HAL_UART_CLEAR_IDLEFLAG(&uart1);
		HAL_UART_AbortReceive_IT(&uart1);
	}
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart) {
    int32_t count = DATA_BUF_SIZE - __HAL_DMA_GET_COUNTER(uart1.hdmarx);
    if (count > 0) {
        memcpy(txbuf, rxbuf, count);
        HAL_UART_Transmit_DMA(&uart1, txbuf, count);
        /**
            通过串口助手发消息控制Led
            连线
            1. ch340 Tx ~ ARx
            2. ATx ~ BTx
            3. AGND ~ BGND
            4. A PA5 ~ Led Low, Led High ~ 3v3
            5. B PA5 ~ Led Low, Led High ~ 3v3
        */
        // if rxbuf == "Close" or if rxbuf == "Open"
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
    HAL_UART_Receive_DMA(&uart1, rxbuf, DATA_BUF_SIZE);
}

void DMA1_Channel4_IRQHandler(void) {
	HAL_DMA_IRQHandler(uart1.hdmatx);
}
void DMA1_Channel5_IRQHandler(void) {
	HAL_DMA_IRQHandler(uart1.hdmarx);
}