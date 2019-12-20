#if defined(ESP8266) or defined(ESP32) or defined(ROSSERIAL_ARDUINO_TCP) 
   #include "ArduinoTcpHardware.h" 
 #else 
   #include "ArduinoHardware.h" 
#endif 
#include <WiFi.h>
#include <Wire.h>
#include <Servo.h> 
#include <ros.h>
#include <std_msgs/Int16.h>
#include <ackermann_teleop/cmd.h>
#include <analogWrite.h>

const char* ssid = "IvLabs";
const char* password = "theroboticists";

IPAddress server(192, 168, 0, 103); // ip of your ROS server
IPAddress ip_address;
int status = WL_IDLE_STATUS;
long timer = 0;

WiFiClient client;

class WiFiHardware {

  public:
  WiFiHardware() {};

  void init() {
    // do your initialization here. this probably includes TCP server/client setup
    client.connect(server, 11411);
  }

  // read a byte from the serial port. -1 = failure
  int read() {
    // implement this method so that it reads a byte from the TCP connection and returns it
    //  you may return -1 is there is an error; for example if the TCP connection is not open
    return client.read();         //will return -1 when it will works
  }

  // write data to the connection to ROS
  void write(uint8_t* data, int length) {
    // implement this so that it takes the arguments and writes or prints them to the TCP connection
    for(int i=0; i<length; i++)
      client.write(data[i]);
  }

  // returns milliseconds since start of program
  unsigned long time() {
     return millis(); // easy; did this one for you
  }
};


int b_left1 = 6;
int b_left2 = 7;
int sp_pwm = 9;
static const int servoPin = 4;

ros::NodeHandle_<WiFiHardware> nh;


Servo servo;

void servo_cb( const ackermann_teleop::cmd& cmd){
  servo.write(int(cmd.steering_angle)); //set servo angle, should be from 0-180   
  Serial.println(int(cmd.speed)); 
  
  if (cmd.speed == 1)
  {
    digitalWrite(b_left1, HIGH);
    digitalWrite(b_left2, LOW);
    analogWrite(sp_pwm, 255);
  }
  if (cmd.speed == -1)
  {
    digitalWrite(b_left1, LOW);
    digitalWrite(b_left2, HIGH);
    analogWrite(sp_pwm, 255);
  }
  if (cmd.speed == 0)
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
  Serial.begin(9600);
  
  nh.initNode();
  nh.subscribe(sub);
  
  servo.attach(servoPin); //attach it to pin 4
}

void loop(){
  nh.spinOnce();
  
  delay(1);
}
