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
int sp_pwm = 9;

//ros::NodeHandle nh;
ros::NodeHandle_<ArduinoHardware, 1, 1, 100, 100> nh;
//ros::NodeHandle_<ArduinoHardware, 1, 1, 150, 150> nh;

Servo servo;

void servo_cb( const ackermann_teleop::cmd& cmd){
  servo.write(int(cmd.steering_angle)); //set servo angle, should be from 0-180  
  digitalWrite(13, HIGH-digitalRead(13));  //toggle led 
  Serial.println(int(cmd.speedy));
  Serial.println(int(cmd.steering_angle)); 
  
  if (cmd.speedy == -1)
  {
    digitalWrite(b_left1, HIGH);
    digitalWrite(b_left2, LOW);
    analogWrite(sp_pwm, 255);
  }
  if (cmd.speedy == 1)
  {
    digitalWrite(b_left1, LOW);
    digitalWrite(b_left2, HIGH);
    analogWrite(sp_pwm, 255);
  }
  if (cmd.speedy == 0)
  {
    digitalWrite(b_left1, LOW); 
    digitalWrite(b_left2, LOW);
    analogWrite(sp_pwm, 0);
  }
  
  delay(1);
}


ros::Subscriber<ackermann_teleop::cmd> sub("servo", &servo_cb);

void setup(){
  pinMode(13, OUTPUT);
  pinMode(b_left1, OUTPUT);
  pinMode(b_left2, OUTPUT);
  Serial.begin(57600);
  
  nh.initNode();
  nh.subscribe(sub);
  
  servo.attach(10); //attach it to pin 9
}

void loop(){
  nh.spinOnce();
  
  delay(1);
}
