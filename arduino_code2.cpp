#include<math.h>
#include<Servo.h>

const float a1=9, a2=9, a3=9, z=4;  //in cenitmeters

Servo servo1, servo2, servo3, servo4;

float* armAngles(float x, float y){

    float r1 = sqrt(x*x + y*y); 
    float r2 = z-a1;
    float r3 = sqrt(r1*r1 + r2*r2);

    float phi1 = acos((a3*a3 - a2*a2 - r3*r3)/(-2*a2*a3));
    float phi2 = atan(r2/r1);
    float phi3 = acos((r3*r3 - a2*a2 - a3*a3)/(-2*a3*a2));

    float theta[4];

    theta[0] = atan(y/x);
    theta[1] = phi1+phi2;
    theta[2] = 180 - phi3;

    return theta;
}

void setup(){
    servo1.attach(13);
    servo1.attach(12);
    servo1.attach(11);
    servo1.attach(10);

    float *theta = armAngles(x,y); //x & y object coordinates
    servo1.write(theta[0]);
    servo2.write(theta[1]);
    servo3.write(theta[2]);
}

void loop(){}