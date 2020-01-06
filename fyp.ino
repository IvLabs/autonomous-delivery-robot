
#include <Servo.h>
#include<Wire.h>
Servo mys;
const int MPU6050_addr=0x68;
int16_t AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;
int b_left1 = 6;
int b_left2 = 7;
int sp_pwm = 9;

int x = 0;
int pos = 90; 

void setup() {
mys.attach(10);
pinMode(b_left1, OUTPUT);
pinMode(b_left2, OUTPUT);
pinMode(sp_pwm, OUTPUT);
Serial.begin(9600);
mys.write(90);
  Wire.begin();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void loop() {

 if (Serial.available())
  {
    x = Serial.read() ;
  
  if (x == 53)   // FORWARD*/
     {
      //Serial.print(x);
      digitalWrite(b_left1, HIGH);
      digitalWrite(b_left2, LOW);
      analogWrite(sp_pwm, 255);
      
      }
  if (x == 52)   // REVERSE
     {
      //Serial.print(x);
      digitalWrite(b_left1, LOW);
      digitalWrite(b_left2, HIGH);
      analogWrite(sp_pwm, 255);
      }


  if (x == 51) // STOP
     {
      //Serial.print(x);
      digitalWrite(b_left1, LOW); //h
      digitalWrite(b_left2, LOW);
      analogWrite(sp_pwm, 0);
      }

 if (x == 50) // right
    {
      //Serial.print(x);
      if(pos > 30)
      {
        pos = pos - 20;
        mys.write(pos);
      } 
    }

 if (x == 49) // left
    {
      //Serial.print(x);
     if( pos < 150)
      {
        pos = pos + 20;
        mys.write(pos);  
      }
    }

  delay(1);   

  }
 Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
  //Temp=Wire.read()<<8|Wire.read();
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
  delay(100);
}
