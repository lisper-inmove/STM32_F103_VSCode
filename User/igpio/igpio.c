#include "igpio.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"

void Led1_Init(void) {
    GPIO_InitTypeDef gpio;
    GPIO_CLK_ENABLE_BY_PORT(Led1_GPIO_Group);
    gpio.Pin = Led1_GPIO_Pin;
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(Led1_GPIO_Group, &gpio);
}