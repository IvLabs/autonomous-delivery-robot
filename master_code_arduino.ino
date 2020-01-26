
#include<ros.h>
#include<geometry_msgs/Point32.h>

#include "DynamixelMotor.h"

const long unsigned int baudrate=1000000;
HardwareDynamixelInterface interface(Serial2);
DynamixelMotor motor(interface,0);

int led = 13;
int b_left1 = 8;
int b_left2 = 9;
int sp_pwm = 10;

ros::NodeHandle nh;
float x, y, z ;

void messageCb( const geometry_msgs::Point32& control_msg){
 
  x = control_msg.x; // fwd or rev 1 or 0
  y = control_msg.y; // steer angle -90 to 90
  z = control_msg.z; // pwm 1 or 0
}

ros::Subscriber<geometry_msgs::Point32> sub("control_input", &messageCb );


void setup() {
 interface.begin(baudrate);
 nh.initNode();
 nh.subscribe(sub);
 pinMode(led, OUTPUT); 
 pinMode(b_left1, OUTPUT);
 pinMode(b_left2, OUTPUT);
 pinMode(sp_pwm, OUTPUT);
 Serial.begin(9600);
 Serial1.begin(57600);
}

void loop() {
   if (y>=-440&&y<=440){
      Bot_Steer(y);
   }

  if (x == 1 && z == 1)   // FORWARD*/
     {
       digitalWrite(led, HIGH); 
      digitalWrite(b_left1, HIGH);
      digitalWrite(b_left2, LOW);
      digitalWrite(sp_pwm, HIGH);
     
     }
      
  if (x == 0 && z == 1)   // REVERSE
     {
      //Serial.print(x);
      digitalWrite(b_left1, LOW);
      digitalWrite(b_left2, HIGH);
      digitalWrite(sp_pwm, HIGH);
      }


  if (z == 0) // STOP
     {
      //Serial.print(x);
      digitalWrite(b_left1, LOW); //h
      digitalWrite(b_left2, LOW);
      digitalWrite(sp_pwm, LOW);
      }

    //Serial1.println("spinning"); 
    //Serial1.println(x);
  nh.spinOnce(); 
  delay(1);
   digitalWrite(led, LOW); 
}


void Bot_Steer(long int ang){
  motor.enableTorque();
  motor.jointMode(1909,2791);
  motor.speed(512);
  long int pos;
  pos = ang+2350;
  motor.goalPosition(pos);
}

