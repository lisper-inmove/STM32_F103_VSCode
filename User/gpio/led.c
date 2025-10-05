#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "led.h"
#include "main.h"

void LED_Init(void) {
    GPIO_InitTypeDef GPIO_InitType;
    GPIO_CLK_ENABLE_BY_PORT(LED_GPIO_Port);
    
    // 0引脚配置
    GPIO_InitType.Pin = LED_Pin;
    // 推挽输出
    GPIO_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    // GPIO_SPEED_FREQ_MEDIUM：中等速度。
    // GPIO_SPEED_FREQ_HIGH：高速。
    // GPIO_SPEED_FREQ_VERY_HIGH：非常高速。
    // 低速，降低电源消耗，响应时间较长
    GPIO_InitType.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitType);
}
