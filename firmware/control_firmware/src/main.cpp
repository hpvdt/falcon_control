#include <Servo.h>
#include <Wire.h>


Servo myservo0;  // create servo object to control a servo
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;

int P1 = 0; // set variables to pots
int P2 = 0;
int P3 = 0;
int P4 = 0;


float angle [3] = {0};              // variable to hold the angle for the servo motor
float input [3] = {0};
float sum0, sum1, sum2, sum3;
float matrix [4][3] = {{90, 0, 0},{0,90,0},{0,0,90},{0,90,90}};

float readings[4];






void receiveEvent(int numberReceived) {
  
  String data0 = "";
  String data1 = "";
  String data2 = "";

  int dataWire = 1;
  while( Wire.available()){
    char dataChar = (char) Wire.read();
    if (dataChar != (char) 'c') {
      // Serial.print("Adding ");
      // Serial.println(dataChar);
      if (dataWire == 1) {
        data0 += dataChar;
        // Serial.println("updated data0");

      }
      else if (dataWire == 2) {
        data1 += dataChar;
        // Serial.println("updated data1");

      }
      else { //Data wire = 3
        data2 += dataChar;
        // Serial.println("updated data2");
      }
    }
    else {
      dataWire += 1;
      // Serial.println("increased Wire");
      // Serial.println(data0);
      // Serial.println(data1);

    }
  }


  readings[0] = data0.toFloat();
  readings[1] = data1.toFloat();
  readings[2] = data2.toFloat();
 
}

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // function that executes whenever data is received from writer
  Serial.begin(115200); //number of bits per second

  myservo0.attach(PB12);  // attaches the servo on pin 25 to the servo object
  myservo1.attach(PB13);
  myservo2.attach(PB14);
  myservo3.attach(PB15);
  myservo4.attach(PB9);

  pinMode(PA5, OUTPUT); //LEDFIELD pins to OUTPUT mode
  pinMode(PA6, OUTPUT);
  pinMode(PA7, OUTPUT);
  pinMode(PB0, OUTPUT);
}

void loop() {
  // myservo.write(0);
  // Serial.println(readings[0]);

  P1 = analogRead(PA3);
  P2 = analogRead(PA2);
  P3 = analogRead(PA1);
  P4 = analogRead(PA0);

  P1 = map(P1, 0, -180, 1023, 180); //map the P1 values to the same range as servo readings
  P2 = map(P2, 0, -180, 1023, 180);
  P3 = map(P3, 0, -180, 1023, 180);
  P4 = map(P4, 0, -180, 1023, 180);
  

  float percent0 = readings[0] / 1023.0;
  float percent1 = readings[1] / 1023.0;
  float percent2 = readings[2] / 1023.0;
  float percent3 = readings[3] / 1023.0;
  float percent4 = readings[4] / 1023.0;

 for (int i = 1; i < 5; i++) {
    Serial.println(i);
     sum0 += matrix[0][i] * readings[0];
     sum1 += matrix[1][i] * readings[1];
     sum2 += matrix[2][i] * readings[2];
     sum3 += matrix[3][i] * readings[3];
   }
   angle[0] = sum0;
   angle[1] = sum1;
   angle[2] = sum2;
   angle[3] = sum3;
   
   Serial.println(angle[0]);
   Serial.println(angle[1]);
   Serial.println(angle[2]);
   Serial.println(angle[3]);
   
  myservo0.write(angle[0]);                  // sets the servo position according to the scaled value
  myservo1.write(angle[1]);
  myservo2.write(angle[2]);
  myservo3.write(angle[3]);

  if (P1 != angle[0]){
    digitalWrite(PA5, HIGH);
  } if (P2 != angle[0]){
    digitalWrite(PA6, HIGH);
  } if (P3 != angle[0]){
    digitalWrite(PA7, HIGH);
  } if (P4 != angle[0]){
    digitalWrite(PB0, HIGH);
  }
  
  delay(200);
}



/* float convertToServo(float reading) {
  // Serial.println((reading / 100 - 1) * 90  + 90);
  return (reading * 90 + 89);
} */
