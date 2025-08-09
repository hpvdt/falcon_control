#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include "pin_allocations.hpp"
#include "control_surfaces.hpp"
#include "flight_stick_comms.hpp"
#include "flight_stick_state.h"

HardwareSerial debug_serial(PIN_DEBUG_RX, PIN_DEBUG_TX);

const int I2C_ADDRESS = 8;
TwoWire main_i2c(PIN_MAIN_SDA, PIN_MAIN_SCL);
TwoWire controller_i2c(PIN_CONTROLLER_SDA, PIN_CONTROLLER_SCL);
const uint8_t PILOT_ADDRESS = 0x23;

const int LED_ACTIVE = HIGH;
const int BUZZER_ACTIVE = HIGH;

// Contribution of RPY to each of the control surfaces
float contribution_matrix[CTRL_SRFC_COUNT][3] ={{  1.000,  0.000,  0.000}, 
                                                {  0.000,  1.000,  0.000}, 
                                                {  0.000,  0.000,  1.000}, 
                                                {  0.000,  1.000,  0.000}}; 

const unsigned long SURFACE_ALIGNMENT_ALERT_HOLD_MS = 2000;

void setup() {
    main_i2c.begin(I2C_ADDRESS);
    debug_serial.begin(115200);

    fs_setup_to_request(&main_i2c);

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
    const unsigned long CURRENT_MS = millis();

    struct FlightStickState user_input;
    fs_read_complete(PILOT_ADDRESS, &user_input);

    float surface_angle[CTRL_SRFC_COUNT] = {0};
    for (int s = 0; s < CTRL_SRFC_COUNT; s++) {
        surface_angle[s] =  contribution_matrix[s][0] * (float)user_input.axis_pitch;
        surface_angle[s] += contribution_matrix[s][1] * (float)user_input.axis_roll;
        surface_angle[s] += contribution_matrix[s][2] * (float)user_input.axis_yaw;
    }

    for (int i = 0; i < CTRL_SRFC_COUNT; i++) ctrl_srfc_set((enum ControlSurface) i, surface_angle[i]);

    // Add some buzzing if any surface is still out of line after a timeout
    // Timeout is needed to ignore transitions
    bool mis_match[CTRL_SRFC_COUNT];
    ctrl_srfc_check_state_all(mis_match);

    static unsigned long unaligned_since_ms[CTRL_SRFC_COUNT];
    bool buzzer_on = false;
    for (int i = 0; i < CTRL_SRFC_COUNT; i++) {
        if (mis_match[i] == false) {
            unaligned_since_ms[i] = 0;
            continue;
        }

        if (unaligned_since_ms == 0) unaligned_since_ms[i] = CURRENT_MS;
        if (CURRENT_MS - unaligned_since_ms[i] > SURFACE_ALIGNMENT_ALERT_HOLD_MS) buzzer_on = true;
    }
    if (buzzer_on) digitalWrite(PIN_BUZZER, BUZZER_ACTIVE);
    else digitalWrite(PIN_BUZZER, !BUZZER_ACTIVE);
    
    delay(20);
}

