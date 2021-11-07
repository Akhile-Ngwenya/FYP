/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>
#include <math.h>

Servo myservo;  // create servo objects to control servo
Servo myservo2;
// twelve servo objects can be created on most boards

int pos = 0;
float height =0;// variable to store the servo position
float len =0; // variable to store slot length
int deg =0; // variable for degrees of rotation
int t_delay=0; // variable for calculated time delays 
void setup() {

  // attaching PWM pins to the servo objects 
  myservo.attach(3);
  myservo2.attach(11);// attaches the servo on pin 9 to the servo object

  // setting up serial plotter
  Serial.begin(9600);

  // making sure that both of the servo motors  are at stand still at the start
  myservo.write(90);
  myservo2.write(90);
}

void lift() {
  Serial.println("Enter desired height in millimeters limit(560 mm)");
  while (Serial.available() == 0) {
    // Wait for User to Input Data
  }
  // height is stored as a variable
  height = Serial.parseFloat();
  // get the length to be travelled in the slot 
  len = 115.6*(height/565);
  // get the desired angle of rotation 
  deg = int(360*(len/115.6));
  Serial.println("");
  Serial.print(deg);
  Serial.println("");
  //calculating the required time delay
  t_delay = int(180*(int(360*(len/115.6) )/60))*(4.6/3);
  Serial.print(t_delay);
  // wait 4 seconds for the user
  delay(4000);
  // then lift the platform
  myservo.write(0);
  myservo2.write(0);
  delay(t_delay);
  myservo.write(90);
  myservo2.write(90);

  // after 3 seconds return the pulley to its origional position
  delay(3000);
  myservo.write(180);
  myservo2.write(180);
  delay(t_delay);
  myservo.write(90);
  myservo2.write(90);

  // reset everyting to zero
  int pos = 0;
  float height =0;// variable to store the servo position
  float len =0;
  int deg =0;
  t_delay =0;}

  void loop() {
  lift();
  
}
