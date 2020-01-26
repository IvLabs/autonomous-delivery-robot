#include "DynamixelMotor.h"

const long unsigned int baudrate=1000000;
HardwareDynamixelInterface interface(Serial);
DynamixelMotor motor(interface,0);

void setup() {
  interface.begin(baudrate);
}

void loop() {

 for(int i=0;i<44;i++){
  bot_steer(-440+20*i);
 delay(500);
 }
 for(int i=0;i<44;i++){
  bot_steer(440-20*i);
 delay(500);
 }

}

void bot_steer(long int ang){
  motor.enableTorque();
  motor.jointMode(1909,2791);
  motor.speed(512);
  long int pos;
  pos = ang+2350;
  motor.goalPosition(pos);
}
