#include <ros.h>
#include<geometry_msgs/Point32.h>
#include "DynamixelMotor.h"

// Proximity Sensor Analog Pins
#define prx0 A0
#define prx1 A1
#define prx2 A2

const long unsigned int baudrate=1000000;
HardwareDynamixelInterface interface(Serial2);
DynamixelMotor motor(interface,0);


int led = 13;
int b_left1 = 8;
int b_left2 = 9;
int sp_pwm = 10;

bool prox_flag;

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

  prox_flag = Proximity_flag(int Prx0,int Prx1,int Prx2);

 if (prox_flag==0){
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
 else{


//manual mode emergency condition  
  }
}


void Bot_Steer(long int ang){
  motor.enableTorque();
  motor.jointMode(1909,2791);
  motor.speed(512);
  long int pos;
  pos = ang+2350;
  motor.goalPosition(pos);
}


bool Proximity_flag(int Prx0,int Prx1,int Prx2){
  
  bool flag0 = 0;
  bool flag1 = 0;
  bool flag2 = 0;
  float dist;


// 5v
  float volts0 = analogRead(prx0)*0.0048828125;  // value from sensor * (5/1024)
  int distance0 = 13*pow(volts0, -1);// worked out from datasheet graph

  float volts1 = analogRead(prx1)*0.0048828125;  // value from sensor * (5/1024)
  int distance1 = 13*pow(volts1, -1);

   float volts2 = analogRead(prx2)*0.0048828125;  // value from sensor * (5/1024)
  int distance2 = 13*pow(volts2, -1);

  
  if (distance0 <= 25){
    flag0=1;
    dist =  get_dist(0,flag0);
    //Serial.print("sensor_0:");
    //Serial.println(dist);   // print the distance
    flag0=0;
  }
  else if (distance1 <= 25){
    flag1=1;
    dist =  get_dist(1,flag1);
    //Serial.print("sensor_1:");
    //Serial.println(dist);   // print the distance
    flag1=0;
  }   
  else if (distance2 <= 25){
    flag2=1;
    dist =  get_dist(2,flag2);
    //Serial.print("sensor_2:");
    //Serial.println(dist);   // print the distance
    flag2=0;
  }

  return(flag0 || flag1 || flag2 )
}

int get_dist(int prx, bool flag){
  float dist; float out;int dly=160;float v;
  out=0;
  if(flag){
   for (int i=0;i<5;i++){
    if(prx==0){
       v = analogRead(prx0)*0.0048828125;
    }
    else if(prx==1){
       v = analogRead(prx1)*0.0048828125;
    }
    else if(prx==2){
       v = analogRead(prx2)*0.0048828125;
    }
    dist = 13*pow(v, -1);
    out = out+dist;
    delay(dly);
   }
   out = out/5;
   return out;
}
else{
  return 100;
}
}


