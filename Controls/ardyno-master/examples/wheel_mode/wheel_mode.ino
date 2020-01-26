// Test motor wheel mode

#include "DynamixelMotor.h"

// id of the motor
const uint8_t id=1;
// speed, between -1023 and 1023
int16_t speed=512;
// communication baudrate
const long unsigned int baudrate = 1000000;

// hardware serial without tristate buffer
// see blink_led example, and adapt to your configuration
HardwareDynamixelInterface interface(Serial);

DynamixelMotor motor(interface, id);

void setup()
{ 
  interface.begin(baudrate);
  delay(100);
  
  // check if we can communicate with the motor
  // if not, we turn the led on and stop here
  uint8_t status=motor.init();
  if(status!=DYN_STATUS_OK)
  {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    while(1);
  }

  motor.enableTorque();  
  motor.wheelMode();
}

//change motion direction every 5 seconds
void loop() 
{
  motor.speed(speed);
  speed=-speed;
  delay(5000);
}

