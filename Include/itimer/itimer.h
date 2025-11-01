#ifndef __TIMER_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_tim_ex.h"

void Timer1_Init(uint16_t arr, uint16_t psc, uint8_t rep);
void Timer2_Init(uint16_t arr, uint16_t psc);
void Timer3_Init(uint16_t arr, uint16_t psc);
void Timer4_Init(uint16_t arr, uint16_t psc);

extern TIM_HandleTypeDef tim1;
extern TIM_HandleTypeDef tim2;
extern TIM_HandleTypeDef tim3;
extern TIM_HandleTypeDef tim4;

#endif