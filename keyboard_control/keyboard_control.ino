#include <SPI.h>
#include <HighPowerStepperDriver.h>
#include <Stepper.h>

const uint8_t CSPin = 4;
const uint16_t StepPeriodUs = 1000;
HighPowerStepperDriver link1;
bool motorControl[3] = {false, false, false};

#define STEPS 256
Stepper nema23(200, 42, 43, 44, 45);
Stepper nema17(200, 10, 11, 12, 13);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  nema23.setSpeed(30);
  nema17.setSpeed(60);

  SPI.begin();
  link1.setChipSelectPin(CSPin);

  // Give the driver some time to power up.
  delay(1);

  // Reset the driver to its default settings and clear latched status
  // conditions.
  link1.resetSettings();
  link1.clearStatus();

  // Select auto mixed decay.  TI's DRV8711 documentation recommends this mode
  // for most applications, and we find that it usually works well.
  link1.setDecayMode(HPSDDecayMode::AutoMixed);

  // Set the current limit. You should change the number here to an appropriate
  // value for your particular system.
  link1.setCurrentMilliamps36v4(3000);

  // Set the number of microsteps that correspond to one full step. 200 ful steps in revolution
  link1.setStepMode(HPSDStepMode::MicroStep256);

  // Enable the motor outputs.
  link1.disableDriver();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {

    // read incoming serial data:
    
    char inChar = Serial.read();
    if(inChar == '1'){ // Nema 34
      link1.enableDriver();
      motorControl[0] = true;
      motorControl[1] = false;
      motorControl[2] = false;
    }
    else if(inChar == '2'){ // Nema 23
      link1.disableDriver();
      motorControl[0] = false;
      motorControl[1] = true;
      motorControl[2] = false;
    }
    else if(inChar == '3'){ // Nema 17
      link1.disableDriver();
      motorControl[0] = false;
      motorControl[1] = false;
      motorControl[2] = true;
    }
    
    if(inChar == 'w'){
      if(motorControl[0]==true){
        link1.setDirection(0);
        for(unsigned int x = 0; x < 256; x++) // does one full step (256 microsteps). 200 full steps for one rev
        {
          link1.step();
          delayMicroseconds(StepPeriodUs);
        }
      }
      else if(motorControl[1]==true){
        nema23.step(1);
        //setStepperIdle(1);
      }
      else if(motorControl[2]==true){
        nema17.step(1);
        delay(5);
        //setStepperIdle(2);
      }
    }
    else if(inChar == 's'){

      if(motorControl[0]==true){
        link1.setDirection(1);
        for(unsigned int x = 0; x < 256; x++)
        {
          link1.step();
          delayMicroseconds(StepPeriodUs);
        }
      }
      else if(motorControl[1]==true){
        nema23.step(-1);
        //setStepperIdle(1);
      }
      else if(motorControl[2]==true){
        nema17.step(-1);
        delay(5);
        //setStepperIdle(2);
      }
    }
    
    //delay(300);
    //mitigateHeat();
    //delay(100);
  }
}

void setStepperIdle(int whichOne){
  if(whichOne == 1){ // Nema 23
    digitalWrite(42, LOW);
    digitalWrite(43, LOW);
    digitalWrite(44, LOW);
    digitalWrite(45, LOW);
  }
  else if(whichOne == 2){ //Nema 17
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }
}
void mitigateHeat(){
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(42, LOW);
  digitalWrite(43, LOW);
  digitalWrite(44, LOW);
  digitalWrite(45, LOW);
}
void setStepperActive(int whichOne){
  if(whichOne == 1){ //Nema 23
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    digitalWrite(44, HIGH);
    digitalWrite(45, HIGH);
  }
  else if(whichOne == 2){ //Nema 17
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
  }
}
