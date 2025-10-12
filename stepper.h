#ifndef STEPPER_CONTROL_H
#define STEPPER_CONTROL_H

#include "main.h"

//记得改这里的脚
#define A1_GPIO GPIOA
#define A1_PIN  GPIO_PIN_1
#define A2_GPIO GPIOA
#define A2_PIN  GPIO_PIN_2
#define B1_GPIO GPIOA
#define B1_PIN  GPIO_PIN_3
#define B2_GPIO GPIOA
#define B2_PIN  GPIO_PIN_4

void Stepper_Control(uint32_t step_delay_ms, int32_t steps);

#endif
