#include<math.h>
#include<Servo.h>
  //in cenitmeters
float coordinates[2];
const float a1=9, a2=9, a3=9, z=4;

Servo servo1, servo2, servo3, servo4;

float* armAngles(float x, float y){

    float r1 = sqrt(x*x + y*y); 
    float r2 = z-a1;
    float r3 = sqrt(r1*r1 + r2*r2);

    float phi1 = acos((a3*a3 - a2*a2 - r3*r3)/(-2*a2*r3));
    printf("phi1: %f\n",phi1);
    float phi2 = atan(r2/r1);
    printf("phi2: %f\n",phi2);
    float phi3 = acos((r3*r3 - a2*a2 - a3*a3)/(-2*a3*a2));

    static float theta[4];
    float param = 180/(2*acos(0.0));
    theta[0] = atan(y/x)*param;
    if (phi2>=0){
        theta[1] = (phi2-phi1)*param;
    }
    else{
        theta[1] = (phi1-phi2)*param;
    }
    theta[2] = 180 - (phi3*param);

    return theta;
}

void setup(){
    servo1.attach(13);
    servo2.attach(12);
    servo3.attach(11);
    servo4.attach(10);
    Serial.begin(9600);
}

void loop()
{
  if(Serial.available()>0){
    for(int i=0;i<2;i++){
      coordinates[i]=Serial.parseFloat();
      delay(500);
      }
    Serial.println(coordinates[0]);
    Serial.println(coordinates[1]);
    float *servo_angles=armAngles(coordinates[0],coordinates[1]);
    Serial.println("Angles:");
    Serial.println(servo_angles[0]);
    Serial.println(servo_angles[1]);
    Serial.println(servo_angles[2]);
      servo1.write(servo_angles[0]);
      servo2.write(servo_angles[1]);
      servo3.write(servo_angles[2]);
    }
}