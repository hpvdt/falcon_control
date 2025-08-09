#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include "pin_allocations.hpp"
#include "control_surfaces.hpp"

HardwareSerial debug_serial(PIN_DEBUG_RX, PIN_DEBUG_TX);

const int I2C_ADDRESS = 8;
TwoWire main_i2c(PIN_MAIN_SDA, PIN_MAIN_SCL);
TwoWire controller_i2c(PIN_CONTROLLER_SDA, PIN_CONTROLLER_SCL);

const int LED_ACTIVE = HIGH;
const int BUZZER_ACTIVE = HIGH;

float angle [4] = {0};
float input [3] = {0};
float sum0, sum1, sum2, sum3;
float matrix [4][3] = {{90, 0, 0},{0,90,0},{0,0,90},{0,90,90}};

float readings[4];


void receive_event(int numberReceived) {
  
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
  main_i2c.begin(I2C_ADDRESS);
  main_i2c.onReceive(receive_event);

  debug_serial.begin(115200);

  ctrl_srfc_init(&debug_serial);

  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED_STATUS_1, OUTPUT);
  pinMode(PIN_LED_STATUS_2, OUTPUT);
  
  digitalWrite(PIN_BUZZER, BUZZER_ACTIVE);
  digitalWrite(PIN_LED_STATUS_1, LED_ACTIVE);
  digitalWrite(PIN_LED_STATUS_2, LED_ACTIVE);
  delay(2000);
  digitalWrite(PIN_BUZZER, !BUZZER_ACTIVE);
  digitalWrite(PIN_LED_STATUS_1, !LED_ACTIVE);
  digitalWrite(PIN_LED_STATUS_2, !LED_ACTIVE);
}

void loop() {

  for (int i = 0; i < 3; i++) {
    sum0 += matrix[0][i] * readings[0];
    sum1 += matrix[1][i] * readings[1];
    sum2 += matrix[2][i] * readings[2];
    sum3 += matrix[3][i] * readings[3];
  }
  angle[0] = sum0;
  angle[1] = sum1;
  angle[2] = sum2;
  angle[3] = sum3;

  for (int i = 0; i < CTRL_SRFC_COUNT; i++) ctrl_srfc_set((enum ControlSurface) i, angle[i]);

  ctrl_srfc_check_state_all();
  
  delay(200);
}

