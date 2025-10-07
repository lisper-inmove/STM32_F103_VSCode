#include "sw.h"
#include "cmsis_gcc.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_gpio_ex.h"
#include "stm32f1xx_hal_rcc.h"

/**
	GPIO_MODE_INPUT: 输入模式，GPIO引脚用于接收外部信号。
		GPIO_PULLUP: 上拉输入（输入模式时，可以选择上拉或下拉电阻）。
		GPIO_PULLDOWN: 下拉输入。
	GPIO_MODE_OUTPUT_PP: 推挽输出模式（Push-Pull）。常用于驱动LED、继电器等设备。
		输出时，GPIO引脚会有两个状态：高电平（驱动到VCC）和低电平（驱动到地）。
	GPIO_MODE_OUTPUT_OD: 开漏输出模式（Open-Drain）。通常用于I2C通信或需要多个设备共享线的场景。
		该模式下输出只能是低电平或高阻态（不驱动）。
	GPIO_MODE_AF_PP: 复用推挽输出模式。用于外设（如UART、SPI等）的推挽输出。
		用于外设模式时，GPIO引脚可以配置为与其他外设连接。
	GPIO_MODE_AF_OD: 复用开漏输出模式。通常用于外设的开漏输出。
	GPIO_MODE_ANALOG: 模拟模式。GPIO引脚在此模式下用于模拟信号输入或输出，不能用于数字操作。
	GPIO_MODE_IT_RISING: 外部中断触发模式（上升沿触发）。用于外部中断。
	GPIO_MODE_IT_FALLING: 外部中断触发模式（下降沿触发）。
	GPIO_MODE_IT_RISING_FALLING: 外部中断触发模式（上升沿和下降沿触发）。
	GPIO_MODE_EVT_RISING: 外部事件触发模式（上升沿触发）。
	GPIO_MODE_EVT_FALLING: 外部事件触发模式（下降沿触发）。
	GPIO_MODE_EVT_RISING_FALLING: 外部事件触发模式（上升沿和下降沿触发）。
*/

/**
	1. GPIO_NOPULL
		含义：不启用上下拉电阻。
		作用：该选项表示GPIO引脚没有连接内部的上拉或下拉电阻。
			引脚的状态依赖于外部电路或外部设备的驱动。
			如果引脚没有外部电路驱动，可能会处于浮动状态（即悬空状态），这会导致不稳定的输入信号。
		适用场景：
			当你希望GPIO引脚的电平由外部电路来控制时使用。
			适用于输出驱动模式，或者外部电路已经决定了输入引脚的电平。

	2. GPIO_PULLUP
		含义：启用上拉电阻。上拉电阻接了一个Vdd（高电平）
		作用：当引脚悬空时（如果有按钮的话，就是按钮抬起），片上外设为高电平。Vdd的电平状态传给了片上外设。
			如果有按钮的话，按钮应该接地，这样按下按钮时，才能改变片上外设的状态
		适用场景：
			用于按钮、开关等输入设备，确保输入引脚在未按下按钮或未接入信号时有一个已知的高电平状态。

	3. GPIO_PULLDOWN
		含义：启用下拉电阻。下拉电阻接地
		作用：与PULLUP相反
		适用场景：用于按钮、开关等输入设备，确保输入引脚在未按下按钮或未接入信号时有一个已知的低电平状态。
*/

uint8_t sw_pressed = 0;

void SW_Init(void) {
	GPIO_InitTypeDef GPIO_InitType;
	GPIO_CLK_ENABLE_BY_PORT(SW_GPIO_Port);
	GPIO_InitType.Pin = SW_Pin;
	GPIO_InitType.Mode = GPIO_MODE_INPUT;
	GPIO_InitType.Pull = GPIO_PULLDOWN;
	// GPIO_InitType.Pull = GPIO_PULLUP;
	// GPIO_InitType.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(SW_GPIO_Port, &GPIO_InitType);
	
	HAL_GPIO_WritePin(SW_GPIO_Port, SW_Pin, GPIO_PIN_RESET);
}

uint8_t SW_Scan_DOWN() {
	/**
		按下触发来控制SW8
	*/
	uint32_t i = 0;
	if (SW_IN == 1 && sw_pressed == 0) {
		for (i = 0; i < 0x7FFF; i++) { // 消抖
			if (SW_IN == 0) {
				return 0;
			}
		}
		sw_pressed = 1;
		return 8;
	} else if (SW_IN == 0) {
		sw_pressed = 0;
	}
	return 0;
}

uint8_t SW_Scan_UP(void) {
	/**
		抬起触发来控制SW8
	*/
	uint32_t i = 0;
	if (SW_IN == 1 && sw_pressed == 0) {
		for (i = 0; i < 0x7FFF; i++) { // 消抖
			if (SW_IN == 0) {
				return 0;
			}
		}
		sw_pressed = 1;
		return 0;
	} else if (SW_IN == 0 && sw_pressed == 1) {
		sw_pressed = 0;
		return 8;
	}
	return 0;
}

uint8_t SW_Scan_LONG_DOWN(void) {
	/**
		长按触发来控制SW8
	*/
	uint32_t i = 0;
	if (SW_IN == 1 && sw_pressed == 0) {
		for (i = 0; i < 0x7FFFFF; i++) { // 增加循环的长度来达到延时触发
			if (SW_IN == 0) {
				return 0;
			}
		}
		sw_pressed = 1;
		return 8;
	} else if (SW_IN == 0) {
		sw_pressed = 0;
	}
	return 0;
}

uint8_t SW_Scan_UP_DELAY(uint32_t delay) {
	/**
		抬起后经过延时触发来控制SW8
		delay: 单位毫秒
	*/
	uint32_t i = 0;
	if (SW_IN == 1 && sw_pressed == 0) {
		for (i = 0; i < 0x7FFF; i++) { // 消抖
			if (SW_IN == 0) {
				return 0;
			}
		}
		sw_pressed = 1;
		return 0;
	} else if (SW_IN == 0 && sw_pressed == 1) {
		sw_pressed = 0;
		HAL_Delay(delay);
		return 8;
	}
	return 0;
}

// =========================== 通过中断控制按键 ==================================
/**
	RISING: 片上外设遇到一个上升沿时触发中断（FALLING则是下降沿）
	PULLDOWN: 下拉电阻有效（片上外设默认为低电平）此时，引脚需要接一个高电平。
		按下时，片上外设 产生一个上升沿，保持按下则保持高电平。
		抬起时，片上外设产生一个低电平，不再按下则保持低电平。
	PULLUP: 上拉电阻有效，片上外设默认为高电平，引脚需要接一个低电平。
		对于按钮的按下和抬起与PULLDOWN相反
*/
void SW_Init_IT() {
	GPIO_InitTypeDef GPIO_InitType;
	GPIO_CLK_ENABLE_BY_PORT(SW_GPIO_Port);
	GPIO_InitType.Pin = SW_Pin;
	// 按键中断，上升延触发
	GPIO_InitType.Mode = GPIO_MODE_IT_RISING;
	// GPIO_InitType.Mode = GPIO_MODE_IT_FALLING;
	// GPIO_InitType.Pull = GPIO_PULLDOWN;
	GPIO_InitType.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(SW_GPIO_Port, &GPIO_InitType);
	
	/** 
		EXTI0_IRQn: 引脚编号为0的那条外部中断线。
			其它的还有 EXTI1_IRQn，EXTI2_IRQn，EXTI3_IRQn，EXTI4_IRQn
			EXTI9_5_IRQn 5~9
			EXTI15_10_IRQn 10~15
	*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	uint32_t i;
	if(SW_IN == 1) {
		for (i = 0; i < 0x7FFF; i++) {
			if (SW_IN == 0) return;
		}
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		__SEV();
	}
}

void EXTI0_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(SW_Pin);
}

// =========================== 通过事件控制按键 ==================================

void SW_Init_Event(void) {
	GPIO_InitTypeDef GPIO_InitType;
	GPIO_CLK_ENABLE_BY_PORT(SW_GPIO_Port);
	GPIO_InitType.Pin = SW_Pin;
	GPIO_InitType.Mode = GPIO_MODE_EVT_RISING;
	GPIO_InitType.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(SW_GPIO_Port, &GPIO_InitType);
}

// =========================== EventOut ==================================

void SW_Init_Event_Out(void) {
	GPIO_InitTypeDef GPIO_InitType;
	GPIO_CLK_ENABLE_BY_PORT(SW_GPIO_Port);
	GPIO_InitType.Pin = Event_Out_GPIO_Pin;
	GPIO_InitType.Mode = GPIO_MODE_AF_PP;
	GPIO_InitType.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SW_GPIO_Port, &GPIO_InitType);

	__HAL_RCC_AFIO_CLK_ENABLE();
	HAL_GPIOEx_ConfigEventout(AFIO_EVENTOUT_PORT_A, AFIO_EVENTOUT_PIN_3);
	HAL_GPIOEx_EnableEventout();
}