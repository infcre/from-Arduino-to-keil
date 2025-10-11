#ifndef STEPPER_H
#define STEPPER_H

#include "stm32f1xx_hal.h"  // 确保 GPIO 和 HAL 函数可用

#ifdef __cplusplus
extern "C" {
#endif

// ---------------- 引脚定义 ----------------
// 封装一个 GPIO 引脚：端口 + 引脚编号
typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
} Pin;

// ---------------- 步进电机结构体（4线版） ----------------
typedef struct {
    uint16_t number_of_steps;     // 每圈总步数（如 200 表示 1.8°/step）
    uint8_t pin_count;            // 固定为 4
    uint32_t step_delay;          // 每步延迟（单位：微秒）
    uint8_t direction;            // 运行方向：1 = 正转，0 = 反转
    uint32_t last_step_time;      // 上一次步进的时间（micros）

    // 四个控制引脚（IN1 ~ IN4）
    Pin motor_pin_1;  // 如 A相 或 IN1
    Pin motor_pin_2;  // 如 B相 或 IN2
    Pin motor_pin_3;  // 如 /A相 或 IN3
    Pin motor_pin_4;  // 如 /B相 或 IN4
} Stepper;

// ---------------- 函数声明 ----------------

/**
 * @brief 初始化一个 4 线步进电机对象
 * @param stepper 指向 Stepper 结构体的指针
 * @param steps_per_revolution 电机每转的步数（例如：200 表示 1.8°/步）
 * @param pin1 第一相控制引脚（如 IN1）
 * @param pin2 第二相控制引脚（如 IN2）
 * @param pin3 第三相控制引脚（如 IN3）
 * @param pin4 第四相控制引脚（如 IN4）
 */
void Stepper_init(Stepper* stepper,
                  uint16_t steps_per_revolution,
                  Pin pin1,
                  Pin pin2,
                  Pin pin3,
                  Pin pin4);

/**
 * @brief 设置电机转速（RPM，转/分钟）
 * @param stepper 指向 Stepper 对象的指针
 * @param rpm 目标转速（大于 0）
 */
void Stepper_setSpeed(Stepper* stepper, uint16_t rpm);

/**
 * @brief 执行指定步数的移动（阻塞式）
 * @param stepper 指向 Stepper 对象的指针
 * @param steps_to_move 要移动的步数（正为正转，负为反转）
 */
void Stepper_step(Stepper* stepper, int steps_to_move);

// ---------------- 辅助函数（可选实现） ----------------
// 若需非阻塞控制，可后续添加 step() 的非阻塞版本或 run() 函数

#ifdef __cplusplus
}
#endif

#endif // STEPPER_H
