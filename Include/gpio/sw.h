#ifndef __SW_H
#define __SW_H

#include <stdint.h>

#define SW_PIN GPIO_PIN_15
#define SW_GPIO_GROUP GPIOC
#define LED_PIN GPIO_PIN_6
#define LED_GPIO_GROUP GPIOA

#define SW_IN HAL_GPIO_ReadPin(SW_GPIO_GROUP, SW_PIN)

void SW_Init(void);
uint8_t SW_Scan_DOWN(void);
uint8_t SW_Scan_LONG_DOWN(void);
uint8_t SW_Scan_UP(void);
uint8_t SW_Scan_UP_DELAY(uint32_t delay);

#endif