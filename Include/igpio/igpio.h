#ifndef __IGPIO_H
#define __IGPIO_H

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

#endif