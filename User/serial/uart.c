#include "uart.h"
#include "main.h"
#include "stm32f1xx_hal_gpio_ex.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_uart.h"

UART_HandleTypeDef uart;
uint8_t txbuff[64], rxbuff[64];
uint8_t rxstate = 0;

void UART_Init(uint32_t baudRate) {
	uart.Instance = USART_Port;
	uart.Init.BaudRate = baudRate;
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.Mode = UART_MODE_TX_RX;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;

    HAL_UART_Init(&uart);
    #if ENABLE_USART1_IT
    HAL_UART_Receive_IT(&uart,rxbuff,UART1_RX_SIZE);
    #endif
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef GPIO_InitType;
    #if ENABLE_USART1_REMAAP
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_USART1_ENABLE();
    #endif
    GPIO_CLK_ENABLE_BY_PORT(Serial_GPIO_Port);
    USART_CLK_ENABLE_BY_PORT(USART_Port);

    GPIO_InitType.Pin = Serial_GPIO_TX_Pin;
    GPIO_InitType.Mode = GPIO_MODE_AF_PP;
    GPIO_InitType.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(Serial_GPIO_Port, &GPIO_InitType);

    GPIO_InitType.Pin = Serial_GPIO_RX_Pin;
    GPIO_InitType.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitType.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(Serial_GPIO_Port, &GPIO_InitType);

    #if ENABLE_USART1_IT
    HAL_NVIC_SetPriority(USART1_IRQn,3,0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
    #endif

    #if ENABLE_USART1_IDLE_IT
    __HAL_UART_ENABLE_IT(&uart, UART_IT_IDLE);
    #endif
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		memcpy(txbuff, rxbuff, 20);
		rxstate = 1;
		HAL_UART_Receive_IT(&uart,rxbuff,20);
	}
}

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&uart);
    if (__HAL_UART_GET_FLAG(&uart, UART_FLAG_IDLE)) {
     	__HAL_UART_CLEAR_IDLEFLAG(&uart);
        uint32_t count = UART1_RX_SIZE - uart.RxXferCount;
        memcpy(txbuff, rxbuff, count);
        HAL_UART_Transmit_IT(&uart,txbuff,count);
        HAL_UART_AbortReceive_IT(&uart);
    }
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart) {
    HAL_UART_Receive_IT(&uart,rxbuff,UART1_RX_SIZE);
}