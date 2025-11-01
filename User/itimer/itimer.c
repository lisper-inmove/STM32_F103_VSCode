#include "itimer.h"
#include "stm32f1xx_hal_tim.h"

TIM_HandleTypeDef tim1;
TIM_HandleTypeDef tim2;
TIM_HandleTypeDef tim3;
TIM_HandleTypeDef tim4;

/**
    psc为预分频器

    主频为72M时，Timer1_Init(1999, 35999, 5)时：
        定时器时钟周期 = 720000000 / 36000 = 2000HZ，即每一秒钟2000个时钟周期
        arr从0 到 1999（TIM_COUNTERMODE_UP），每一个时钟周期 arr 的值被修改一次，当从0到1999时，刚好2000个时钟周期 1秒钟
        rep是基于0的。每完成一次从 0 ~ arr，算一次，当达到 (rep + 1) 次后触发一次 Update 事件
*/

void Timer1_Init(uint16_t arr, uint16_t psc, uint8_t rep) {
    tim1.Instance = TIM1;
    tim1.Init.Prescaler = psc;
    /**
        向上计数: 从0计数到 arr，触发一次，一共 arr + 1个计数
        向下计数: 从arr计数到 0，触发一次，一共 arr + 1个计数
        中心计数: 从 0 ~ arr ~ 0，触发一次，一共 arr 个计数
    */
    tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim1.Init.Period = arr;
    tim1.Init.RepetitionCounter = rep;
    /**
        计时期间，修改arr的值，是即刻生效还是下次生效，DISABLE为即刻生效
    */
    tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        
    /*
        HAL_TIM_Base_Init 函数中会调用 TIM_Base_SetConfig，SetConfig函数中有以内容

        // Generate an update event to reload the Prescaler(预分频器的值)
        // and the repetition counter (only for advanced timer) value immediately
        TIMx->EGR = TIM_EGR_UG;
    */
    HAL_TIM_Base_Init(&tim1);
    /**
        HAL库会软件产生一次更新事件，所以在Start之后，会把上收到一次完成（uprintf函数打印一次）
        使用clear宏手动清除，可以去掉此逻辑
        加上这一句之后，再观察现在，TIM2, TIM3, TIM4 在启动之后马上输出 1，TIM1不会
    */
    __HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start(&tim1);
}

void Timer2_Init(uint16_t arr, uint16_t psc) {
	tim2.Instance = TIM2;
	tim2.Init.Prescaler = psc;
	tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2.Init.Period = arr;
	tim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim2);
    __HAL_TIM_CLEAR_FLAG(&tim2, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start(&tim2);
}
void Timer3_Init(uint16_t arr, uint16_t psc) {
	tim3.Instance = TIM3;
	tim3.Init.Prescaler = psc;
	tim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim3.Init.Period = arr;
	tim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim3);	
    __HAL_TIM_CLEAR_FLAG(&tim3, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start(&tim3);
}
void Timer4_Init(uint16_t arr, uint16_t psc) {
	tim4.Instance = TIM4;
	tim4.Init.Prescaler = psc;
	tim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim4.Init.Period = arr;
	tim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim4);	
    __HAL_TIM_CLEAR_FLAG(&tim4, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start(&tim4);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM1) {
		__HAL_RCC_TIM1_CLK_ENABLE();
	}else if(htim->Instance == TIM2) {
		__HAL_RCC_TIM2_CLK_ENABLE();
	}else if(htim->Instance == TIM3) {
		__HAL_RCC_TIM3_CLK_ENABLE();
	}else if(htim->Instance == TIM4) {
		__HAL_RCC_TIM4_CLK_ENABLE();
	}
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM1) {
		__HAL_RCC_TIM1_CLK_DISABLE();
	}else if(htim->Instance == TIM2) {
		__HAL_RCC_TIM2_CLK_DISABLE();
	}else if(htim->Instance == TIM3) {
		__HAL_RCC_TIM3_CLK_DISABLE();
	}else if(htim->Instance == TIM4) {
		__HAL_RCC_TIM4_CLK_DISABLE();
	}
}