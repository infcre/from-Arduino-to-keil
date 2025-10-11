#pragma once
#ifndef Stepper_h
#define Stepper_h
#include "stm32f1xx_hal.h"

struct Pin {
    GPIO_TypeDef* GPIOx;
    uint16_t      GPIO_Pin;
};

static inline void digitalWrite(const struct Pin p, GPIO_PinState state) {
    HAL_GPIO_WritePin(p.GPIOx, p.GPIO_Pin, state);
}


/*
程序用到了Arduino里面的micros()，要自己做一个
在CubeMX里面把TIM2的Prescaler设成 CPU频率-1 MHz，这样才有1微秒
static inline uint32_t micros(void)
{
    return __HAL_TIM_GET_COUNTER(&htim2);
}
粘贴进main.c里面
*/

typedef struct {
Pin pins[5];
    int pin_count;
    long step_number;
    long speed;
    long step_delay;
    long last_step_time;
} Stepper;

// 函数声明
void Stepper_init(Stepper *Stepper, Pin *pins, int pin_count);
void setSpeed(Stepper *Stepper, int rpm);
void step(Stepper *Stepper, int steps);
unsigned long micros(void);

#endif /* STEPPER_H */

