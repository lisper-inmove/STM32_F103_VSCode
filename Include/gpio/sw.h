#ifndef __SW_H
#define __SW_H

#include <stdint.h>
#include "main.h"

#define SW_IN HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin)

void SW_Init(void);
void SW_Init_IT(void);
void SW_Init_Event(void);
uint8_t SW_Scan_DOWN(void);
uint8_t SW_Scan_LONG_DOWN(void);
uint8_t SW_Scan_UP(void);
uint8_t SW_Scan_UP_DELAY(uint32_t delay);

#endif