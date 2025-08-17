#include <Arduino.h>
#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;

int i;
  
void setup() {
  Serial.begin(115200); //number of bits per second
  myservo1.attach(25);  // attaches the servo on pin PB12 to the servo object
  myservo2.attach(26); //PB13
  myservo3.attach(27); //PB14
  myservo4.attach(28); //PB15
  myservo5.attach(46); //PB9
}

void loop() {
  // put your main code here, to run repeatedly:
  for (i = 0; i <= 180; i += 1){
    //myservo1.write(i);
    //myservo2.write(i);
    //myservo3.write(i);
    //myservo4.write(i);
    myservo5.write(i);
    delay(15);
  }
  for (i = 180; i >= 0; i -= 1){
    //myservo1.write(i);
    //myservo2.write(i);
    //myservo3.write(i);
    //myservo4.write(i);
    myservo5.write(i);
    delay(15);
  }
}
