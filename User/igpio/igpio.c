#include "igpio.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_cortex.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"

/**
    GPIO 8种 工作模式:

    4种输入模式:
	GPIO_MODE_INPUT: 输入模式，GPIO引脚用于接收外部信号。
		GPIO_PULLUP: 上拉输入（输入模式时，可以选择上拉或下拉电阻）。
		GPIO_PULLDOWN: 下拉输入。    
	GPIO_MODE_ANALOG: 模拟输入。GPIO引脚在此模式下用于模拟信号输入或输出，不能用于数字操作。
    浮空输入：上电时的默认值，没有一个确定的高电平或者低电平

    4种输出:
	GPIO_MODE_OUTPUT_PP: 推挽输出模式（Push-Pull）。
	GPIO_MODE_OUTPUT_OD: 开漏输出模式（Open-Drain）。
	GPIO_MODE_AF_PP: 复用推挽输出模式。
	GPIO_MODE_AF_OD: 复用开漏输出模式。通常用于外设的开漏输出。

	GPIO_MODE_IT_RISING: 外部中断触发模式（上升沿触发）。用于外部中断。
	GPIO_MODE_IT_FALLING: 外部中断触发模式（下降沿触发）。
	GPIO_MODE_IT_RISING_FALLING: 外部中断触发模式（上升沿和下降沿触发）。
	GPIO_MODE_EVT_RISING: 外部事件触发模式（上升沿触发）。
	GPIO_MODE_EVT_FALLING: 外部事件触发模式（下降沿触发）。
	GPIO_MODE_EVT_RISING_FALLING: 外部事件触发模式（上升沿和下降沿触发）。
*/


uint8_t SWPressed = 0;

void Led1_Init(void) {
    GPIO_InitTypeDef gpio;
    GPIO_CLK_ENABLE_BY_PORT(Led1_GPIO_Group);
    gpio.Pin = Led1_GPIO_Pin;
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(Led1_GPIO_Group, &gpio);
}

void SW1_Init(void) {
    GPIO_InitTypeDef gpio;
    GPIO_CLK_ENABLE_BY_PORT(SW1_GPIO_Group);
    gpio.Pin = SW1_GPIO_Pin;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(SW1_GPIO_Group, &gpio);
}

void SW1_Scan_Down(void) {
    /**
        按下立即触发
    */
    if (SW1_IN == 1 && SWPressed == 0) {
        for (int i = 0; i < 0xFFFF; i++) { // 消抖
            if (SW1_IN == 0) {
                return;
            }
        }
        HAL_GPIO_TogglePin(Led1_GPIO_Group, Led1_GPIO_Pin);
        SWPressed = 1;
    } else if (SW1_IN == 0 && SWPressed == 1) {
        SWPressed = 0;
    }
}

void SW1_Scan_Long_Down(void) {
    /**
        长按: 增加 SW1_Scan_Down消抖的时长即可
    */
}
void SW1_Scan_Up(void) {
    /**
        抬起触发
    */
    if (SW1_IN == 1 && SWPressed == 0) {
        for (int i = 0; i < 0xFFFF; i++) { // 消抖
            if (SW1_IN == 0) {
                return;
            }
        }
        SWPressed = 1;
    } else if (SW1_IN == 0 && SWPressed == 1) {
        SWPressed = 0;
        HAL_GPIO_TogglePin(Led1_GPIO_Group, Led1_GPIO_Pin);
    }
}
void SW1_Scan_Up_Delay(void) {
    /**
        抬起之后延时触发
    */
    if (SW1_IN == 1 && SWPressed == 0) {
        for (int i = 0; i < 0xFFFF; i++) { // 消抖
            if (SW1_IN == 0) {
                return;
            }
        }
        SWPressed = 1;
    } else if (SW1_IN == 0 && SWPressed == 1) {
        HAL_Delay(3000);
        SWPressed = 0;
        HAL_GPIO_TogglePin(Led1_GPIO_Group, Led1_GPIO_Pin);
    }
}

// ============== 用中断来控制 =====================


void SW1_Init_IT(void) {
    GPIO_InitTypeDef gpio;
    GPIO_CLK_ENABLE_BY_PORT(SW1_GPIO_Group);
    gpio.Pin = SW1_GPIO_Pin;
    gpio.Mode = GPIO_MODE_IT_RISING;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(SW1_GPIO_Group, &gpio);
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler(void) {
    // 此函数由汇编代码调用（GPIO 10~15的中断）
	HAL_GPIO_EXTI_IRQHandler(SW1_GPIO_Pin);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (SW1_IN == 1) {
        for (int i = 0; i < 0xFFFF; i++) {
            if (SW1_IN == 0) return;
        }
        HAL_GPIO_TogglePin(Led1_GPIO_Group, Led1_GPIO_Pin);
    }
}