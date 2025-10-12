#include "stepper.h"

//这个是ULN2003 + 28BYJ-48用的，有四根线，别的可能用不了
const uint8_t sequence[8][4] = {
    {1, 0, 0, 0},  // IN1
    {1, 1, 0, 0},  // IN1+IN2
    {0, 1, 0, 0},  // IN2
    {0, 1, 1, 0},  // IN2+IN3
    {0, 0, 1, 0},  // IN3
    {0, 0, 1, 1},  // IN3+IN4
    {0, 0, 0, 1},  // IN4
    {1, 0, 0, 1}   // IN4+IN1
};

void Stepper_Control(uint32_t step_delay_ms, int32_t steps) {
    uint8_t current_step = 0;
    uint8_t direction = (steps < 0) ? 1 : 0;
    steps = (steps < 0) ? -steps : steps;

    for (uint32_t i = 0; i < steps; i++) {                                 
        if (direction == 0) {
            current_step = (current_step + 1) % 8;
        } else {
            current_step = (current_step + 7) % 8;
        }

        HAL_GPIO_WritePin(A1_GPIO, A1_PIN, sequence[current_step][0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(A2_GPIO, A2_PIN, sequence[current_step][1] ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(B1_GPIO, B1_PIN, sequence[current_step][2] ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(B2_GPIO, B2_PIN, sequence[current_step][3] ? GPIO_PIN_SET : GPIO_PIN_RESET);

        HAL_Delay(step_delay_ms);
    }
	HAL_GPIO_WritePin(A1_GPIO, A1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(A2_GPIO, A2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(B1_GPIO, B1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(B2_GPIO, B2_PIN, GPIO_PIN_RESET);

}
