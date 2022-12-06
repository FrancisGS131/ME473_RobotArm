#include <Stepper.h>

// change this to the number of steps on your motor
#define STEPS 100

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper1(STEPS, 42, 43, 44, 45);


// the previous reading from the analog input
int previous = 0;

void setup() {
  // set the speed of the motor to 30 RPMs
  stepper1.setSpeed(30);
  Serial.begin(9600);
}

void loop() {
  // get the sensor value
  int val = analogRead(A0);
  Serial.println(val);

  // move a number of steps equal to the change in the
  // sensor reading
  stepper1.step(val - previous);

  // remember the previous value of the sensor
  previous = val;
}

// ---------------------------------------------
void controlStepper(){
  // Stuff inside 
}

void controlServo() {
  // Stuff inside
}
