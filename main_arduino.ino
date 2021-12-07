#include<math.h>
#include<Servo.h>
#include<string.h>

  //in cenitmeters
float coordinates[2];
const float a1=7, a2=9.6, a3=9.6;

Servo servo1, servo2, servo3, servo4;
String first(String s)
{
    String ans="";
    for(int i=0; i<s.length(); i++)
    {
        if(s[i]==',') break;
        ans = ans + s[i];
    }
    return ans;
}
String second(String s)
{
    String ans="";
    for(int i=s.length()-1; i>=0; i--)
    {
        if(s[i]==',') break;
        ans=s[i]+ans;
    }
    return ans;
}

float* armAngles(float x, float y, float z){
    
    float r1 = sqrt(x*x + y*y); 
    float r2 = a1-z;
    float r3 = sqrt(r1*r1 + r2*r2);

    float phi1 = acos((a3*a3 - a2*a2 - r3*r3)/(-2*a2*r3));
    printf("phi1: %f\n",phi1);
    float phi2 = atan(r2/r1);
    printf("phi2: %f\n",phi2);
    float phi3 = acos((r3*r3 - a2*a2 - a3*a3)/(-2*a3*a2));

    static float theta[4];
    float param = 180/(2*acos(0.0));
    theta[0] = atan(y/x)*param;
    theta[1] = 100 - (phi1-phi2)*param;
    theta[2] = 180 - (phi3*param);

    return theta;
}

void setup(){
    servo1.attach(13);
    servo2.attach(12);
    servo3.attach(11);
    servo4.attach(10);

    servo3.write(0);
    servo4.write(0);
    Serial.begin(9600);
}

void destiny(){
  servo3.write(0);
  delay(2000);
  servo1.write(180);
  delay(1000);
  servo4.write(20);
  delay(1000);
  servo1.write(93);
  delay(1000);
 
}

void moveUpDown(float x, float y, float z){
    
    Serial.println("Angle when z (in pick())");
    float *servo_angles=armAngles(x,y,z);
    servo1.write(servo_angles[0]);
    delay(3000);
    servo2.write(servo_angles[1]);
    delay(1000);
    servo3.write(servo_angles[2]);
    delay(1000);

    Serial.println("Angle when z (in pick())");
    Serial.println(servo_angles[0]);
    Serial.println(servo_angles[1]);
    Serial.println(servo_angles[2]);
}


void loop()
{
  if(Serial.available()>0){
    lastDelay:
    String input = Serial.readStringUntil('\n');
    String x = first(input);
    String y = second(input);
    float fx =atof(x.c_str());
    float fy =atof(y.c_str());
    coordinates[0]= fx;
    coordinates[1]= fy;

    float radius = sqrt(fx*fx + fy*fy);
    Serial.println(fx);
    Serial.println(fy);
    Serial.println(radius);
    if(fx<=0 && fy <=0){
       goto lastDelay;
    }
    else if( radius >= 16.4 && radius <= 17.8) {
      Serial.println("1. I'm working");
      moveUpDown(coordinates[0], coordinates[1], 1);
      moveUpDown(coordinates[0], coordinates[1], 5);
      servo4.write(180);
      moveUpDown(coordinates[0], coordinates[1], 1);
      destiny();
    } else if( radius >= 15.0 && radius < 16.4) {
      Serial.println("2. I'm working");
      moveUpDown(coordinates[0], coordinates[1], -1);
      moveUpDown(coordinates[0], coordinates[1], 3);
      servo4.write(180);
      moveUpDown(coordinates[0], coordinates[1], -1);
      destiny();
    }else{
      Serial.println("3. I'm working");
      moveUpDown(coordinates[0], coordinates[1], -4);
      moveUpDown(coordinates[0], coordinates[1], 0);
      servo4.write(180);
      delay(3000);
      moveUpDown(coordinates[0], coordinates[1], -4);
      destiny();
    }
      
    delay(800);
    
    }
}
