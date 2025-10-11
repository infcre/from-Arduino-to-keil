#include "Stepper.h"
#include <stdlib.h>
#define HIGH GPIO_PIN_SET
#define LOW  GPIO_PIN_RESET

void Stepper_init(Stepper *Stepper,int number_of_steps,
                 struct Pin pin1,struct Pin pin2,struct Pin pin3,struct Pin pin4,struct Pin pin5)
{
  Stepper->step_number = 0;    // which step the motor is on
  Stepper->direction = 0;      // motor direction
  Stepper->last_step_time = 0; // time stamp in us of the last step taken
  Stepper->number_of_steps = number_of_steps; // total number of steps for Stepper motor

  // Arduino pins for the motor control connection:
  Stepper->motor_pin_1 = pin1;
  Stepper->motor_pin_2 = pin2;
  Stepper->motor_pin_3 = pin3;
  Stepper->motor_pin_4 = pin4;
  Stepper->motor_pin_5 = pin5;

  // pin_count is used by the stepMotor() method:
  Stepper->pin_count = 5;
}

/*
 * Sets the speed in revs per minute
 */
void setSpeed(Stepper *Stepper,long whatSpeed)
{
  Stepper->step_delay = 60L * 1000L * 1000L / Stepper->number_of_steps / whatSpeed;
}

/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void step(Stepper *Stepper,int steps_to_move)
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
