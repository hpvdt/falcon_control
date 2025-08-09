#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include "pin_allocations.hpp"
#include "control_surfaces.hpp"
#include "flight_stick_comms.hpp"

HardwareSerial debug_serial(PIN_DEBUG_RX, PIN_DEBUG_TX);

const int I2C_ADDRESS = 8;
TwoWire main_i2c(PIN_MAIN_SDA, PIN_MAIN_SCL);
TwoWire controller_i2c(PIN_CONTROLLER_SDA, PIN_CONTROLLER_SCL);

const int LED_ACTIVE = HIGH;
const int BUZZER_ACTIVE = HIGH;

float angle [4] = {0};
float input [3] = {0};
float matrix [4][3] = {{90, 0, 0}, {0,90,0}, {0,0,90}, {0,90,90}};

void setup() {
    main_i2c.begin(I2C_ADDRESS);
    debug_serial.begin(115200);

    setup_to_request(&main_i2c);

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

    // for (int i = 0; i < 3; i++) {
    //   sum0 += matrix[0][i] * readings[0];
    //   sum1 += matrix[1][i] * readings[1];
    //   sum2 += matrix[2][i] * readings[2];
    //   sum3 += matrix[3][i] * readings[3];
    // }
    // angle[0] = sum0;
    // angle[1] = sum1;
    // angle[2] = sum2;
    // angle[3] = sum3;

    for (int i = 0; i < CTRL_SRFC_COUNT; i++) ctrl_srfc_set((enum ControlSurface) i, angle[i]);

    ctrl_srfc_check_state_all();
    
    delay(200);
}

