#include "Stepper.h"
#include <stdint.h>

// ---------------- 外部依赖：micros() ----------------
// 请确保在 main.c 或其他地方实现了 micros()
// 示例（CubeMX 配置 TIM2，PSC=71，CNT=72MHz）：
//
// extern TIM_HandleTypeDef htim2;
// static inline uint32_t micros(void) {
//     return __HAL_TIM_GET_COUNTER(&htim2);
// }

// 若你已全局定义 micros()，则无需在此声明
extern uint32_t micros(void);

// ---------------- 内部函数：执行单步 ----------------
// 双四拍（Double Phase Stepping）相序：高转矩，推荐使用
static void stepMotor(Stepper* stepper, int step_num) {
    // 双四拍 4 步序列表（IN1, IN2, IN3, IN4）
    // 顺序：1100 → 0110 → 0011 → 1001
    static const uint8_t step_sequence[4][4] = {
        {1, 1, 0, 0},  // Step 0: IN1=H, IN2=H
        {0, 1, 1, 0},  // Step 1: IN2=H, IN3=H
        {0, 0, 1, 1},  // Step 2: IN3=H, IN4=H
        {1, 0, 0, 1}   // Step 3: IN4=H, IN1=H
    };

    uint8_t out1 = step_sequence[step_num][0];
    uint8_t out2 = step_sequence[step_num][1];
    uint8_t out3 = step_sequence[step_num][2];
    uint8_t out4 = step_sequence[step_num][3];

    HAL_GPIO_WritePin(stepper->motor_pin_1.GPIOx, stepper->motor_pin_1.GPIO_Pin, out1 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(stepper->motor_pin_2.GPIOx, stepper->motor_pin_2.GPIO_Pin, out2 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(stepper->motor_pin_3.GPIOx, stepper->motor_pin_3.GPIO_Pin, out3 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(stepper->motor_pin_4.GPIOx, stepper->motor_pin_4.GPIO_Pin, out4 ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// ---------------- 初始化函数 ----------------
void Stepper_init(Stepper* stepper,
                  uint16_t steps_per_revolution,
                  Pin pin1,
                  Pin pin2,
                  Pin pin3,
                  Pin pin4) {
    // 设置基本参数
    stepper->number_of_steps = steps_per_revolution;
    stepper->pin_count = 4;                    // 固定为 4 线
    stepper->step_delay = 0;                   // 初始无延迟
    stepper->direction = 1;                    // 默认正转
    stepper->last_step_time = 0;               // 尚未开始

    // 绑定引脚
    stepper->motor_pin_1 = pin1;
    stepper->motor_pin_2 = pin2;
    stepper->motor_pin_3 = pin3;
    stepper->motor_pin_4 = pin4;

    // 初始化所有引脚为低电平（安全状态）
    HAL_GPIO_WritePin(pin1.GPIOx, pin1.GPIO_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pin2.GPIOx, pin2.GPIO_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pin3.GPIOx, pin3.GPIO_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pin4.GPIOx, pin4.GPIO_Pin, GPIO_PIN_RESET);
}

// ---------------- 设置转速（RPM） ----------------
void Stepper_setSpeed(Stepper* stepper, uint16_t rpm) {
    if (rpm == 0) {
        rpm = 1;  // 防止除以零，最低速度设为 1 RPM
    }
    // 计算每步延迟（单位：微秒）
    // 公式：delay = (60 * 1000 * 1000) / (steps_per_rev) / (rpm)
    stepper->step_delay = (60L * 1000L * 1000L) / stepper->number_of_steps / rpm;
}

// ---------------- 执行指定步数（阻塞式） ----------------
void Stepper_step(Stepper* stepper, int steps_to_move) {
    int steps_left = steps_to_move > 0 ? steps_to_move : -steps_to_move;  // 步数绝对值
    uint8_t direction = (steps_to_move > 0) ? 1 : 0;  // 1=正转，0=反转

    // 更新方向
    stepper->direction = direction;

    // 当前步序（可从 0~3）
    uint8_t step_num = 0;

    // 循环执行每一步
    while (steps_left > 0) {
        uint32_t now = micros();

        // 只有经过足够延迟才执行下一步
        if (now - stepper->last_step_time >= stepper->step_delay) {
            // 更新最后执行时间
            stepper->last_step_time = now;

            // 根据方向更新步序
            if (direction == 1) {
                step_num = (step_num + 1) % 4;
            } else {
                step_num = (step_num == 0) ? 3 : step_num - 1;
            }

            // 执行当前步
            stepMotor(stepper, step_num);

            // 减少剩余步数
            steps_left--;
        }
    }
}void step(Stepper *Stepper,int steps_to_move)
{
  int steps_left = abs(steps_to_move);  // how many steps to take 

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) { Stepper->direction = 1; }
  if (steps_to_move < 0) { Stepper->direction = 0; }


  // decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {
    unsigned long now = micros();
    // move only if the appropriate delay has passed:
    if (now - Stepper->last_step_time >= Stepper->step_delay)
    {
      // get the timeStamp of when you stepped:
      Stepper->last_step_time = now;
      // increment or decrement the step number,
      // depending on direction:
      if (Stepper->direction == 1)
      {
        Stepper->step_number++;
        if (Stepper->step_number == Stepper->number_of_steps) {
          Stepper->step_number = 0;
        }
      }
      else
      {
        if (Stepper->step_number == 0) {
          Stepper->step_number = Stepper->number_of_steps;
        }
        Stepper->step_number--;
      }
      // decrement the steps left:
      steps_left--;
      // step the motor to step number 0, 1, ..., {3 or 10}
      if (Stepper->pin_count == 5)
        stepMotor(Stepper->step_number % 10);
      else
        stepMotor(Stepper->step_number % 4);
    }
  }
}

/*
 * Moves the motor forward or backwards.
 */
int stepMotor(Stepper *Stepper,int StepperStep)
{
  if (Stepper->pin_count == 5) {
    switch (StepperStep) {
      case 0:  // 01101
        digitalWrite(Stepper->motor_pin_1, LOW);
        digitalWrite(Stepper->motor_pin_2, HIGH);
        digitalWrite(Stepper->motor_pin_3, HIGH);
        digitalWrite(Stepper->motor_pin_4, LOW);
        digitalWrite(Stepper->motor_pin_5, HIGH);
        break;
      case 1:  // 01001
        digitalWrite(Stepper->motor_pin_1, LOW);
        digitalWrite(Stepper->motor_pin_2, HIGH);
        digitalWrite(Stepper->motor_pin_3, LOW);
        digitalWrite(Stepper->motor_pin_4, LOW);
        digitalWrite(Stepper->motor_pin_5, HIGH);
        break;
      case 2:  // 01011
        digitalWrite(Stepper->motor_pin_1, LOW);
        digitalWrite(Stepper->motor_pin_2, HIGH);
        digitalWrite(Stepper->motor_pin_3, LOW);
        digitalWrite(Stepper->motor_pin_4, HIGH);
        digitalWrite(Stepper->motor_pin_5, HIGH);
        break;
      case 3:  // 01010
        digitalWrite(Stepper->motor_pin_1, LOW);
        digitalWrite(Stepper->motor_pin_2, HIGH);
        digitalWrite(Stepper->motor_pin_3, LOW);
        digitalWrite(Stepper->motor_pin_4, HIGH);
        digitalWrite(Stepper->motor_pin_5, LOW);
        break;
      case 4:  // 11010
        digitalWrite(Stepper->motor_pin_1, HIGH);
        digitalWrite(Stepper->motor_pin_2, HIGH);
        digitalWrite(Stepper->motor_pin_3, LOW);
        digitalWrite(Stepper->motor_pin_4, HIGH);
        digitalWrite(Stepper->motor_pin_5, LOW);
        break;
      case 5:  // 10010
        digitalWrite(Stepper->motor_pin_1, HIGH);
        digitalWrite(Stepper->motor_pin_2, LOW);
        digitalWrite(Stepper->motor_pin_3, LOW);
        digitalWrite(Stepper->motor_pin_4, HIGH);
        digitalWrite(Stepper->motor_pin_5, LOW);
        break;
      case 6:  // 10110
        digitalWrite(Stepper->motor_pin_1, HIGH);
        digitalWrite(Stepper->motor_pin_2, LOW);
        digitalWrite(Stepper->motor_pin_3, HIGH);
        digitalWrite(Stepper->motor_pin_4, HIGH);
        digitalWrite(Stepper->motor_pin_5, LOW);
        break;
      case 7:  // 10100
        digitalWrite(Stepper->motor_pin_1, HIGH);
        digitalWrite(Stepper->motor_pin_2, LOW);
        digitalWrite(Stepper->motor_pin_3, HIGH);
        digitalWrite(Stepper->motor_pin_4, LOW);
        digitalWrite(Stepper->motor_pin_5, LOW);
        break;
      case 8:  // 10101
        digitalWrite(Stepper->motor_pin_1, HIGH);
        digitalWrite(Stepper->motor_pin_2, LOW);
        digitalWrite(Stepper->motor_pin_3, HIGH);
        digitalWrite(Stepper->motor_pin_4, LOW);
        digitalWrite(Stepper->motor_pin_5, HIGH);
        break;
      case 9:  // 00101
        digitalWrite(Stepper->motor_pin_1, LOW);
        digitalWrite(Stepper->motor_pin_2, LOW);
        digitalWrite(Stepper->motor_pin_3, HIGH);
        digitalWrite(Stepper->motor_pin_4, LOW);
        digitalWrite(Stepper->motor_pin_5, HIGH);
        break;
    }
  }
}
