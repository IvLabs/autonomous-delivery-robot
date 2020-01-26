// Sharp IR GP2Y0A41SK0F Distance Test
// Sharp IR GP2Y0A41SK0F (4-30cm, analog)

#define prx0 A0
#define prx1 A1
#define prx2 A2

bool flag0 = 0;
bool flag1 = 0;
bool flag2 = 0;
float dist;



void setup() {
  Serial.begin(9600); // start the serial port
}

void loop() {
  
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
    Serial.print("sensor_0:");
    Serial.println(dist);   // print the distance
    flag0=0;
  }
  else if (distance1 <= 25){
    flag1=1;
    dist =  get_dist(1,flag1);
    Serial.print("sensor_1:");
    Serial.println(dist);   // print the distance
    flag1=0;
  }   
  else if (distance2 <= 25){
    flag2=1;
    dist =  get_dist(2,flag2);
    Serial.print("sensor_2:");
    Serial.println(dist);   // print the distance
    flag2=0;
  }
  
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

