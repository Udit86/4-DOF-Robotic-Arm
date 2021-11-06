#include<math.h>
#include<Servo.h>

const float a1=9, a2=9, a3=9, z=4;  //in cenitmeters

Servo servo1, servo2, servo3, servo4;

float r1 = sqrt(x*x + y*y); // x & y object coordinates
float r2 = z-a1;
float r3 = sqrt(r1*r1 + r2*r2);

float phi1 = acos((a3*a3 - a2*a2 - r3*r3)/(-2*a2*a3));
float phi2 = atan(r2/r1);
float phi3 = acos((r3*r3 - a2*a2 - a3*a3)/(-2*a3*a2));

float theta1 = atan(y/x);
float theta2 = phi1+phi2;
float theta3 = 180 - phi3;

void setup(){
    servo1.attach(13);
    servo1.attach(12);
    servo1.attach(11);
    servo1.attach(10);

    servo1.write(theta1);
    servo2.write(theta2);
    servo3.write(theta3);
}

void loop(){}