#ifndef Stepper_h
#define Stepper_h

struct Pin {
    GPIO_TypeDef* GPIOx;
    uint16_t      GPIO_Pin;
};

// library interface description
class Stepper {
  public:
    // constructors:
    Stepper(int number_of_steps,
                 GPIO_TypeDef* port1, uint16_t pin1,
                 GPIO_TypeDef* port2, uint16_t pin2,
                 GPIO_TypeDef* port3, uint16_t pin3,
                 GPIO_TypeDef* port4, uint16_t pin4,
                 GPIO_TypeDef* port5, uint16_t pin5)

    // speed setter method:
    void setSpeed(long whatSpeed);

    // mover method:
    void step(int number_of_steps);

    int version(void);

  private:
    void stepMotor(int this_step);

    int direction;            // Direction of rotation
    unsigned long step_delay; // delay between steps, in ms, based on speed
    int number_of_steps;      // total number of steps this motor can take
    int pin_count;            // how many pins are in use.
    int step_number;          // which step the motor is on

    // motor pin numbers:
    Pin motor_pin_1;
    Pin motor_pin_2;
    Pin motor_pin_3;
    Pin motor_pin_4;
    Pin motor_pin_5;

    unsigned long last_step_time; // time stamp in us of when the last step was taken
};

#endif

