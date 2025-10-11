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
*/


// library interface description
typedef struct {
    int direction;            // Direction of rotation
    unsigned long step_delay; // delay between steps, in ms, based on speed
    int number_of_steps;      // total number of steps this motor can take
    int pin_count;            // how many pins are in use.
    int step_number;          // which step the motor is on

    // motor pin numbers:
    struct Pin motor_pin_1;
    struct Pin motor_pin_2;
    struct Pin motor_pin_3;
    struct Pin motor_pin_4;
    struct Pin motor_pin_5;

    unsigned long last_step_time; // time stamp in us of when the last step was taken
} Stepper;

#endif
