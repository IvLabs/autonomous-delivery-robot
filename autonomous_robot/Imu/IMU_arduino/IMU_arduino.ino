#include<Wire.h>
#include <ros.h>
#include <std_msgs/Float32.h>
#include <autonomous_robot/Imu.h>

const int MPU6050_addr=0x68;
float AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;

ros::NodeHandle_<ArduinoHardware, 1, 1, 100, 100> nh;

autonomous_robot::Imu imu_msg;

ros::Publisher pub("imu", &imu_msg);

void setup() {
  
  nh.initNode();
  nh.advertise(pub);
  
  Wire.begin();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void loop() {
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
  // Temp=Wire.read()<<8|Wire.read();
  GyroX=Wire.read()<<8|Wire.read();
  GyroY=Wire.read()<<8|Wire.read();
  GyroZ=Wire.read()<<8|Wire.read();
  Serial.print("AccX = "); Serial.print(AccX);
  Serial.print(" || AccY = "); Serial.print(AccY);
  Serial.print(" || AccZ = "); Serial.print(AccZ);
 // Serial.print(" || Temp = "); Serial.print(Temp/340.00+36.53);
  Serial.print(" || GyroX = "); Serial.print(GyroX);
  Serial.print(" || GyroY = "); Serial.print(GyroY);
  Serial.print(" || GyroZ = "); Serial.println(GyroZ);

  imu_msg.ax = AccX;
  imu_msg.ay = AccY;
  imu_msg.az = AccZ;
  imu_msg.gx = GyroX;
  imu_msg.gy = GyroY;
  imu_msg.gz = GyroZ;
  
  pub.publish( &imu_msg );
  nh.spinOnce();
  
  delay(100);
}
