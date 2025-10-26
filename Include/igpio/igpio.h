#ifndef __IGPIO_H
#define __IGPIO_H

#define SERIAL_1_GPIO_Group GPIOA
#define SERIAL_1_GPIO_TX_Pin GPIO_PIN_9
#define SERIAL_1_GPIO_RX_Pin GPIO_PIN_10

#define SERIAL_2_GPIO_Group GPIOA
#define SERIAL_2_GPIO_TX_Pin GPIO_PIN_2
#define SERIAL_2_GPIO_RX_Pin GPIO_PIN_3

#define SERIAL_3_GPIO_Group GPIOB
#define SERIAL_3_GPIO_TX_Pin GPIO_PIN_10
#define SERIAL_3_GPIO_RX_Pin GPIO_PIN_11

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