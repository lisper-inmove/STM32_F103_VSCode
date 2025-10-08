#include "uart.h"
#include "main.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_dma.h"
#include "stm32f1xx_hal_gpio_ex.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_uart.h"

UART_HandleTypeDef uart;
uint8_t txbuff[64], rxbuff[64];
uint8_t rxstate = 0;

DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

void UART_Init(uint32_t baudRate) {
	uart.Instance = USART_Port;
	uart.Init.BaudRate = baudRate;
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.Mode = UART_MODE_TX_RX;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;

    HAL_UART_Init(&uart);
    
    #if ENABLE_USART1_DMA
    HAL_UART_Receive_DMA(&uart, rxbuff, UART1_RX_SIZE);
    #elif ENABLE_USART1_IT
    HAL_UART_Receive_IT(&uart, rxbuff, UART1_RX_SIZE);
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
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    #endif

    Set_UART1_DMA(huart);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		memcpy(txbuff, rxbuff, 20);
		rxstate = 1;
		HAL_UART_Receive_IT(&uart,rxbuff,20);
	}
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // 收到一半的数据了，可以使用另一个线程做一些耗时的事情
    }
}

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&uart);
    if (__HAL_UART_GET_FLAG(&uart, UART_FLAG_IDLE)) {
     	__HAL_UART_CLEAR_IDLEFLAG(&uart);
        uint32_t count = UART1_RX_SIZE - __HAL_DMA_GET_COUNTER(uart.hdmarx);
        memcpy(txbuff, rxbuff, count);

        #if ENABLE_USART1_DMA
        HAL_UART_Transmit_DMA(&uart, txbuff, count);
        #elif ENABLE_USART1_IT
        HAL_UART_Transmit_IT(&uart, txbuff, count);
        #endif
        HAL_UART_AbortReceive_IT(&uart);
    }
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart) {
    #if ENABLE_USART1_DMA
    HAL_UART_Receive_DMA(&uart, rxbuff, UART1_RX_SIZE);
    #elif ENABLE_USART1_IT
    HAL_UART_Receive_IT(&uart, rxbuff, UART1_RX_SIZE);
    #endif
}

// ================== DMA 方式实现串口收发消息 ========================
void Set_UART1_DMA(UART_HandleTypeDef *huart) {
    #if ENABLE_USART1_DMA
	__HAL_RCC_DMA1_CLK_ENABLE();

    hdma_usart1_tx.Instance = DMA1_Channel4;
    // 从内存到外设
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    // 外设地址是否自增。串口的数据寄存器 USARTx->DR 地址固定，必须禁用自增
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    // 内存地址是否自增。发送/接收连续缓冲区时都要开自增（ENABLE），这样每传一个字节就读/写内存下一个地址。
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;

    // 	uart.Init.WordLength = UART_WORDLENGTH_8B; 8B使用BYTE，9B使用 HALFWORD
    // 外设端每次传输数据宽度
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    // 内存端每次传输宽度
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;

    // 1) 配好 hdma_usartx_rx.Init.Mode = DMA_CIRCULAR（环形更顺）
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    // 通道优先级
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_MEDIUM;

    // 在__HAL_LINKDMA这个宏里，将 hdmatx 链接到 huart->hdmatx上
    // huart->hdmatx = hdma_usart1_tx;
    // hdma_usart1_tx.parent = huart
    __HAL_LINKDMA(huart, hdmatx, hdma_usart1_tx);
    HAL_DMA_Init(&hdma_usart1_tx);
    // 开始中断，使得 DMA1_Channel4_IRQHandler 被调用
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart1_rx.Init.Mode = DMA_NORMAL;
	hdma_usart1_rx.Init.Priority = DMA_PRIORITY_MEDIUM;

	__HAL_LINKDMA(huart, hdmarx, hdma_usart1_rx);
	HAL_DMA_Init(&hdma_usart1_rx);
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn,3,0);
	HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    #endif
}

void DMA1_Channel4_IRQHandler(void) {
    // 内存到外设
	HAL_DMA_IRQHandler(uart.hdmatx);
}
void DMA1_Channel5_IRQHandler(void) {
    // 外设到内存
	HAL_DMA_IRQHandler(uart.hdmarx);
}
