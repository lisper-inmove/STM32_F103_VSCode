#ifndef __IGPIO_H
#define __IGPIO_H

#define Led1_GPIO_Group GPIOA
#define Led1_GPIO_Pin GPIO_PIN_6

#define SW1_GPIO_Group GPIOC
#define SW1_GPIO_Pin GPIO_PIN_15

#define GPIO_CLK_ENABLE_BY_PORT(port)               \
  do {                                              \
    if ((port) == GPIOA)      { __HAL_RCC_GPIOA_CLK_ENABLE(); } \
    else if ((port) == GPIOB) { __HAL_RCC_GPIOB_CLK_ENABLE(); } \
    else if ((port) == GPIOC) { __HAL_RCC_GPIOC_CLK_ENABLE(); } \
  } while (0)

#define GPIO_CLK_DISABLE_BY_PORT(port)              \
  do {                                              \
    if ((port) == GPIOA)      { __HAL_RCC_GPIOA_CLK_DISABLE(); } \
    else if ((port) == GPIOB) { __HAL_RCC_GPIOB_CLK_DISABLE(); } \
    else if ((port) == GPIOC) { __HAL_RCC_GPIOC_CLK_DISABLE(); } \
  } while (0)

#define SW1_IN HAL_GPIO_ReadPin(SW1_GPIO_Group, SW1_GPIO_Pin)

void Led1_Init(void);
void SW1_Init(void);
void SW1_Init_IT(void);
void SW1_Scan_Down(void);
void SW1_Scan_Long_Down(void);
void SW1_Scan_Up(void);
void SW1_Scan_Up_Delay(void);

#endif