#include <Stepper.h>
const int stepsPerRevolution = 90;
// change this to fit the number of steps per revolution
// for your motor
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, PA0, PA1, PA4, PA7);

void setup() {
// set the speed at 60 rpm:
myStepper.setSpeed(20);
// initialize the serial port:
Serial.begin(115200);
}

void loop() {
// step one revolution in one direction:
Serial.println("clockwise");
myStepper.step(stepsPerRevolution);
delay(500);
// step one revolution in the other direction:
Serial.println("counterclockwise");
myStepper.step(-stepsPerRevolution);
delay(500);
}
