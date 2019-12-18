/*
 * rosserial Servo Control Example
 *
 * This sketch demonstrates the control of hobby R/C servos
 * using ROS and the arduiono
 * 
 * For the full tutorial write up, visit
 * www.ros.org/wiki/rosserial_arduino_demos
 *
 * For more information on the Arduino Servo Library
 * Checkout :
 * http://www.arduino.cc/en/Reference/Servo
 */

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include <WProgram.h>
#endif

#include <Servo.h> 
#include <ros.h>
#include <std_msgs/Int16.h>
#include <ackermann_teleop/cmd.h>

int b_left1 = 6;
int b_left2 = 7;

ros::NodeHandle  nh;

Servo servo;

void servo_cb( const ackermann_teleop::cmd& cmd){
  servo.write(int(cmd.speed)); //set servo angle, should be from 0-180  
  digitalWrite(13, HIGH-digitalRead(13));  //toggle led 
  Serial.println(int(cmd.speed)); 
  
  if (cmd.speed == 1)
  {
    digitalWrite(b_left1, HIGH);
    digitalWrite(b_left2, LOW);
  }
  if (cmd.speed == -1)
  {
    digitalWrite(b_left1, LOW);
    digitalWrite(b_left2, HIGH);
  }
  if (cmd.speed == 0)
  {
    digitalWrite(b_left1, LOW); 
    digitalWrite(b_left2, LOW);
  }
  
  delay(1);
}


ros::Subscriber<ackermann_teleop::cmd> sub("servo_pub", servo_cb);

void setup(){
  pinMode(13, OUTPUT);
  pinMode(b_left1, OUTPUT);
  pinMode(b_left2, OUTPUT);
  Serial.begin(57600);
  
  nh.initNode();
  nh.subscribe(sub);
  
  servo.attach(9); //attach it to pin 9
}

void loop(){
  nh.spinOnce();
  
  delay(1);
}
