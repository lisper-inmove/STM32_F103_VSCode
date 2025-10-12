#ifndef __TIME_H
#define __TIME_H

#include "stm32f103xb.h"
#include "stm32f1xx_hal_tim.h"

extern TIM_HandleTypeDef tim1;
extern TIM_HandleTypeDef tim2;
extern TIM_HandleTypeDef tim3;
extern TIM_HandleTypeDef tim4;


void Timer1_Init(uint16_t arr, uint16_t psc, uint8_t rep);
void Timer2_Init(uint16_t arr, uint16_t psc);
void Timer3_Init(uint16_t arr, uint16_t psc);
void Timer4_Init(uint16_t arr, uint16_t psc);


#endif