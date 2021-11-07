

#include "TimerOne.h"
#include <math.h>
// defining pi as a constant
#define PI 3.1415926535897932384626433832795
// Motor 1 High/ Low pins
int In1 = 7;
int In2 = 8;

// Motor 2 high/low pins
int In3 = 12;
int In4 = 13;

//motor 1 PWM pin
int ENA = 5;
// motor 2 pwm pin
int ENA2 = 6;
int SPEED = 0;

//setpint to zero
int inst,setPoint=0;

//counting number of rotations
float num_rot=0;

// distance covered varible
float dst_cov =0;
//setting all intermediatory variables to zero
float dist,rot=0,rot_cov=0,sp=0,sprad=0,error=0;
// the wheel radius in meters
float w_radius = 0.040;
//output PWM signal
int pwmsig =0;

// counter the interrupts 
float rotation1 =0;
// Constants for interruput pins
const byte motor1 = 2; // motor 1 Interrupt pin - INT 0 

// Integers for pulse counters
unsigned int counter1 =0; 

// number of slots in encoder disk
float diskslots = 1.00;

// Interruput Service Routines
void ISR_count1()
{
  counter1++;
}

void ISR_timerone()
{
  Timer1.detachInterrupt(); //stops the timer
  //Serial.print("Motor Speed");
  float rotation1 = (counter1 / diskslots); // calculate RPM
  // keeps count of the rotations covered
  rot_cov = rot_cov + rotation1;
  // converts the rotations covered to distance covered
  dst_cov += 2*PI*w_radius*rotation1;
  Serial.print(rotation1);
  Serial.print(",");
  Serial.print(dst_cov);
  Serial.println("");
  counter1=0; // reset counter to zero

  Timer1.attachInterrupt( ISR_timerone ); //Enable the timer
}

void rigcontrol(){
  dst_cov =0;
   rot_cov =0;
  Serial.println("Enter Distance to be covered in meters");
  while (Serial.available() == 0) {
  //  Wait for User to Input Data
 }
   dist = Serial.parseFloat();
   rot = (dist/(2*PI*0.04)) //- (2*PI*0.04);
   Serial.println("");
   Serial.println("Enter speed of rig in (m/s) range (5.27-6.5)");
   while (Serial.available() == 0) {
  //  Wait for User to Input Data
   }
  // speed from user
   sp = Serial.parseFloat();
  // transform it to radians/second
   sprad = sp/(2*PI*w_radius);
   setPoint =int(sprad);
   pwmsig = (int)((sprad + 59.6)/1.24);
   Serial.println("");
   Serial.print(pwmsig);
   Serial.println("");
   SPEED = pwmsig;
   delay(20000);
   Timer1.attachInterrupt( ISR_timerone );
   if(dist >=0){
    while(rot>rot_cov){
      // proportional controller
     error =  2.334814815*(setPoint - rotation1);
     SPEED = (int)((error + 59.6)/1.24);
    digitalWrite(In1,HIGH);
    digitalWrite(In2,LOW);

    //sending the pwm signal to the motors 
    analogWrite(ENA,SPEED);
    
    digitalWrite(In3,HIGH);
    digitalWrite(In4,LOW);
    analogWrite(ENA2,SPEED);

    // saturation block
    if(rotation1>=16){
      SPEED=70;
      }
    }
    
    // after the distance is reached set everything else back to zero 
    // stop the motors 
    dst_cov =0;
    rot_cov =0;
    
    digitalWrite(In1,LOW);
    digitalWrite(In2,HIGH);
    digitalWrite(In3,LOW);
    digitalWrite(In4,HIGH);
    delay(100);
    digitalWrite(In1,LOW);
    digitalWrite(In2,LOW);
    digitalWrite(In3,LOW);
    digitalWrite(In4,LOW);
    sp=0;
    sprad=0;
    SPEED =0;
}
  
  
  }
void setup() {
// setting up the serial monitor
Serial.begin(9600);
// seting the timer to sample at 10Hz
Timer1.initialize(1000000/10);
// attaching the interrupt to pin 2 and to detect a falling edge
attachInterrupt(digitalPinToInterrupt(motor1), ISR_count1,FALLING); // Increase counter 1 when speed sensor pin goes low
 // enabling the timer

// setting pins as ouptuts
pinMode(In1,OUTPUT);
pinMode(In2,OUTPUT);
pinMode(In3,OUTPUT);
pinMode(In4,OUTPUT);
pinMode(ENA,OUTPUT);
pinMode(ENA2,OUTPUT);

}

void loop() {
  rigcontrol();
  // put your main code here, to run repeatedly:
}
