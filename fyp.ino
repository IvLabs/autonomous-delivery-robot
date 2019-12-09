
#include <Servo.h>
Servo mys;

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
      if(pos > 0)
      {
        pos = pos - 30;
        mys.write(pos);
      } 
    }

 if (x == 49) // left
    {
      //Serial.print(x);
     if( pos < 180)
      {
        pos = pos + 30;
        mys.write(pos);  
      }
    }

  delay(1);   

  }

}
