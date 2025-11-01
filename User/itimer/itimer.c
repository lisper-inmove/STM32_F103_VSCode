#include "itimer.h"
#include "iserial.h"
#include "stm32f1xx_hal_tim.h"

TIM_HandleTypeDef tim1;
DMA_HandleTypeDef tim1_dma;

TIM_HandleTypeDef tim2;
DMA_HandleTypeDef tim2_dma;

TIM_HandleTypeDef tim3;
DMA_HandleTypeDef tim3_dma;

TIM_HandleTypeDef tim4;
DMA_HandleTypeDef tim4_dma;

uint16_t tim1_dmabuff[4] = {3999, 5999, 7999, 9999};
uint16_t tim2_dmabuff[4] = {1200-1, 1800-1, 2400-1, 3000-1};
uint16_t tim3_dmabuff[4] = {6000-1, 9000-1, 12000-1, 15000-1};
uint16_t tim4_dmabuff[4] = {6600-1, 7600-1, 8600-1, 9600-1};

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
    __HAL_TIM_ENABLE_IT(&tim1, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_DMA(&tim1, (uint32_t *)tim1_dmabuff, 4);
}

void Timer2_Init(uint16_t arr, uint16_t psc) {
	tim2.Instance = TIM2;
	tim2.Init.Prescaler = psc;
	tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2.Init.Period = arr;
	tim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim2);
    __HAL_TIM_CLEAR_FLAG(&tim2, TIM_FLAG_UPDATE);
    __HAL_TIM_ENABLE_IT(&tim2, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_DMA(&tim2, (uint32_t *)tim2_dmabuff, 4);
}

void Timer3_Init(uint16_t arr, uint16_t psc) {
	tim3.Instance = TIM3;
	tim3.Init.Prescaler = psc;
	tim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim3.Init.Period = arr;
	tim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim3);	
    __HAL_TIM_CLEAR_FLAG(&tim3, TIM_FLAG_UPDATE);
    __HAL_TIM_ENABLE_IT(&tim3, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_DMA(&tim3, (uint32_t *)tim3_dmabuff, 4);
}

void Timer4_Init(uint16_t arr, uint16_t psc) {
	tim4.Instance = TIM4;
	tim4.Init.Prescaler = psc;
	tim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim4.Init.Period = arr;
	tim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim4);	
    __HAL_TIM_CLEAR_FLAG(&tim4, TIM_FLAG_UPDATE);
    __HAL_TIM_ENABLE_IT(&tim4, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_DMA(&tim4, (uint32_t *)tim4_dmabuff, 4);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM1) {
		__HAL_RCC_TIM1_CLK_ENABLE();
        __HAL_RCC_DMA1_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM1_UP_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);

        tim1_dma.Instance = DMA1_Channel5;
		tim1_dma.Init.Direction = DMA_MEMORY_TO_PERIPH;
		tim1_dma.Init.PeriphInc = DMA_PINC_DISABLE;
		tim1_dma.Init.MemInc = DMA_MINC_ENABLE;
		tim1_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		tim1_dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		tim1_dma.Init.Mode = DMA_CIRCULAR;
		tim1_dma.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(&tim1, hdma[TIM_DMA_ID_UPDATE], tim1_dma);
		HAL_DMA_Init(&tim1_dma);
		
		HAL_NVIC_SetPriority(DMA1_Channel5_IRQn,3,0);
		HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	}else if(htim->Instance == TIM2) {
		__HAL_RCC_TIM2_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);

		tim2_dma.Instance = DMA1_Channel2;
		tim2_dma.Init.Direction = DMA_MEMORY_TO_PERIPH;
		tim2_dma.Init.PeriphInc = DMA_PINC_DISABLE;
		tim2_dma.Init.MemInc = DMA_MINC_ENABLE;
		tim2_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		tim2_dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		tim2_dma.Init.Mode = DMA_NORMAL;
		tim2_dma.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(&tim2, hdma[TIM_DMA_ID_UPDATE], tim2_dma);
		HAL_DMA_Init(&tim2_dma);
		
		HAL_NVIC_SetPriority(DMA1_Channel2_IRQn,3,0);
		HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

	}else if(htim->Instance == TIM3) {
		__HAL_RCC_TIM3_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
		
		tim3_dma.Instance = DMA1_Channel3;
		tim3_dma.Init.Direction = DMA_MEMORY_TO_PERIPH;
		tim3_dma.Init.PeriphInc = DMA_PINC_DISABLE;
		tim3_dma.Init.MemInc = DMA_MINC_ENABLE;
		tim3_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		tim3_dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		tim3_dma.Init.Mode = DMA_NORMAL;
		tim3_dma.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(&tim3, hdma[TIM_DMA_ID_UPDATE], tim3_dma);
		HAL_DMA_Init(&tim3_dma);
		
		HAL_NVIC_SetPriority(DMA1_Channel3_IRQn,3,0);
		HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

	}else if(htim->Instance == TIM4) {
		__HAL_RCC_TIM4_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM4_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(TIM4_IRQn);

        tim4_dma.Instance = DMA1_Channel7;
		tim4_dma.Init.Direction = DMA_MEMORY_TO_PERIPH;
		tim4_dma.Init.PeriphInc = DMA_PINC_DISABLE;
		tim4_dma.Init.MemInc = DMA_MINC_ENABLE;
		tim4_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		tim4_dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		tim4_dma.Init.Mode = DMA_NORMAL;
		tim4_dma.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(&tim4, hdma[TIM_DMA_ID_UPDATE], tim4_dma);
		HAL_DMA_Init(&tim4_dma);
		
		HAL_NVIC_SetPriority(DMA1_Channel7_IRQn,4,0);
		HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);        
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

int32_t num1 = 0;
int32_t num2 = 0;
int32_t num3 = 0;
int32_t num4 = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM1) {
        if(htim->State == HAL_TIM_STATE_READY) {
            htim->State = HAL_TIM_STATE_BUSY;
		    u1_printf("Timer1 DMA Complete %d\n", num1);
        } else {
            u1_printf("Timer1 Finish %d\n", num1);
		    num1++;
        }
	}else if(htim->Instance == TIM2) {
		if(htim->State == HAL_TIM_STATE_READY) {
            htim->State = HAL_TIM_STATE_BUSY;
		    u1_printf("Timer2 DMA Complete %d\n", num2);
        } else {
            u1_printf("Timer2 Finish %d\n", num2);
		    num2++;
        }
	}else if(htim->Instance == TIM3) {
		if(htim->State == HAL_TIM_STATE_READY) {
            htim->State = HAL_TIM_STATE_BUSY;
		    u1_printf("Timer3 DMA Complete %d\n", num3);
        } else {
            u1_printf("Timer3 Finish %d\n", num3);
		    num3++;
        }
	}else if(htim->Instance == TIM4) {
		if(htim->State == HAL_TIM_STATE_READY) {
            htim->State = HAL_TIM_STATE_BUSY;
		    u1_printf("Timer4 DMA Complete %d\n", num4);
        } else {
            u1_printf("Timer4 Finish %d\n", num4);
		    num4++;
        }
	}
}

void HAL_TIM_PeriodElapsedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1) {
		u1_printf("Timer1 hal finish\n");
		htim->State = HAL_TIM_STATE_BUSY;		
	} else 	if(htim->Instance == TIM2) {
		u1_printf("Timer2 hal finish\n");
		htim->State = HAL_TIM_STATE_BUSY;		
	} else 	if(htim->Instance == TIM3) {
		u1_printf("Timer3 hal finish\n");
		htim->State = HAL_TIM_STATE_BUSY;		
	} else 	if(htim->Instance == TIM4) {
		u1_printf("Timer4 hal finish\n");
		htim->State = HAL_TIM_STATE_BUSY;		
	}
}

void TIM1_UP_IRQHandler(void) {
	HAL_TIM_IRQHandler(&tim1);
}

void DMA1_Channel5_IRQHandler(void) {
	HAL_DMA_IRQHandler(&tim1_dma);
}

void TIM2_IRQHandler(void) {
	HAL_TIM_IRQHandler(&tim2);
}

void DMA1_Channel2_IRQHandler(void) {
	HAL_DMA_IRQHandler(&tim2_dma);
}

void TIM3_IRQHandler(void) {
	HAL_TIM_IRQHandler(&tim3);
}

void DMA1_Channel3_IRQHandler(void) {
	HAL_DMA_IRQHandler(&tim3_dma);
}

void TIM4_IRQHandler(void) {
	HAL_TIM_IRQHandler(&tim4);
}

void DMA1_Channel7_IRQHandler(void) {
	HAL_DMA_IRQHandler(&tim4_dma);
}