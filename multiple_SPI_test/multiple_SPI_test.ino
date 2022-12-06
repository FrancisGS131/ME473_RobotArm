#include <SPI.h>
#include <HighPowerStepperDriver.h>
#include <AMIS30543.h>

const uint8_t CSPin = 5;
const uint16_t StepPeriodUs = 100;
HighPowerStepperDriver link1;

const uint8_t amisDirPin = 2;
const uint8_t amisStepPin = 3;
const uint8_t amisSlaveSelect = 4;
AMIS30543 stepper;

void setup() {
  // put your setup code here, to run once:
  SPI.begin();
  link1.setChipSelectPin(CSPin);
  stepper.init(amisSlaveSelect);

  // Give the driver some time to power up.
  delay(1);

  // Reset the driver to its default settings and clear latched status
  // conditions.
  link1.resetSettings();
  link1.clearStatus();

  stepper.resetSettings();

  // Select auto mixed decay.  TI's DRV8711 documentation recommends this mode
  // for most applications, and we find that it usually works well.
  link1.setDecayMode(HPSDDecayMode::AutoMixed);

  // Set the current limit. You should change the number here to an appropriate
  // value for your particular system.
  link1.setCurrentMilliamps36v4(3000);
  stepper.setCurrentMilliamps(1500);

  // Set the number of microsteps that correspond to one full step. 200 ful steps in revolution
  link1.setStepMode(HPSDStepMode::MicroStep128);
  stepper.setStepMode(AMIS30543::MicroStep128);

  // Enable the motor outputs.
  link1.enableDriver();
  stepper.enableDriver();
}

void loop() {
  // Step in the default direction 1000 times.
  setDirection(0);
  link1.setDirection(0);
  for (unsigned int x = 0; x < 128*200; x++)
  {
    link1.step();
    step();
    //delayMicroseconds(StepPeriodUs);
  }

  // Wait for 300 ms.
  delay(300);

  // Step in the other direction 1000 times.
  setDirection(1);
  link1.setDirection(1);
  for (unsigned int x = 0; x < 128*200; x++)
  {
    link1.step();
    step();
  }

  // Wait for 300 ms.
  delay(300);
}


/** ------------------------------------
 * AMIS Driver Functions
 */ 

// Sends a pulse on the NXT/STEP pin to tell the driver to take
// one step, and also delays to control the speed of the motor.
void step()
{
  // The NXT/STEP minimum high pulse width is 2 microseconds.
  digitalWrite(amisStepPin, HIGH);
  delayMicroseconds(3);
  digitalWrite(amisStepPin, LOW);
  delayMicroseconds(3);

  // The delay here controls the stepper motor's speed.  You can
  // increase the delay to make the stepper motor go slower.  If
  // you decrease the delay, the stepper motor will go fast, but
  // there is a limit to how fast it can go before it starts
  // missing steps.
  delayMicroseconds(100);
}

// Writes a high or low value to the direction pin to specify
// what direction to turn the motor.
void setDirection(bool dir)
{
  // The NXT/STEP pin must not change for at least 0.5
  // microseconds before and after changing the DIR pin.
  delayMicroseconds(1);
  digitalWrite(amisDirPin, dir);
  delayMicroseconds(1);
}
