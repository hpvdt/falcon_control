#include <Arduino.h>
#include <Servo.h>
#include "pin_allocations.hpp"

#include "control_surfaces.hpp"


Servo actuator[NUM_SERVOS];
const int ACTUATOR_DEFAULT = 90;
const int ACTUATOR_MINIMUM = 90;
const int ACTUATOR_MAXIMUM = 90;

float target_angles_deg[4];

float calculate_servo_angle(enum ControlSurface surface, float target) {
    float angle_out;

    // Some math to convert the desired surface angle to a servo based on the linkage system
    switch (surface) {
    case CTRL_SRFC_RGHT_TIP:
        angle_out = target;
        break;
    case CTRL_SRFC_LFT_TIP:
        angle_out = target;
        break;
    case CTRL_SRFC_RDR:
        angle_out = target;
        break;
    case CTRL_SRFC_ELVTR:
        angle_out = target;
        break;
    
    default:
        angle_out = 0;
        break;
    }

    return angle_out;
}

void ctrl_srfc_init(Stream* debug) {
    for (int i = 0; i < NUM_SERVOS; i++) {
        pinMode(PIN_SERVOS[i], OUTPUT);
        actuator[i].attach(PIN_SERVOS[i]);
        actuator[i].write(ACTUATOR_DEFAULT);
    }

    for (int i = 0; i < NUM_LED_FIELDS; i++) {
        pinMode(PIN_LED_FIELD[i], OUTPUT);
    }

    for (int i = 0; i < NUM_POTENTIOMETERS; i++) {
        pinMode(PIN_POTENTIOMETERS[i], INPUT_ANALOG);
    }

    if (debug != nullptr) debug->println(F("Control surfaces setup"));

    if (debug != nullptr) debug->println(F("Control surfaces sweeping"));
    for (int i = 0; i < CTRL_SRFC_COUNT; i++) actuator[i].write(ACTUATOR_MINIMUM);
    delay(1000);
    for (int i = 0; i < CTRL_SRFC_COUNT; i++) actuator[i].write(ACTUATOR_MAXIMUM);
    delay(1000);
    for (int i = 0; i < CTRL_SRFC_COUNT; i++) actuator[i].write(ACTUATOR_DEFAULT);
    delay(1000);
    if (debug != nullptr) debug->println(F("Control surfaces completed sweep"));
}

void ctrl_srfc_set(enum ControlSurface surface, float angle_deg) {
    float angle_out_deg = calculate_servo_angle(surface, angle_deg);

    // Could add some error checking here

    target_angles_deg[surface] = angle_out_deg;
    actuator[surface].write(angle_out_deg);
}

float ctrl_srfc_read_surface(enum ControlSurface query) {
    uint32_t raw = analogRead(PIN_POTENTIOMETERS[query]);
    
    float angle_read_deg = (raw - 512) / (512.0 / 180.0);

    return angle_read_deg;
}

void ctrl_srfc_check_state_all(bool feedback[]) {
    const float ANGLE_TOLERANCE_DEG = 3;

    for (int i = 0; i < CTRL_SRFC_COUNT; i++) {
        float target_deg = target_angles_deg[i];
        float current_deg = ctrl_srfc_read_surface((enum ControlSurface) i);

        float delta_deg = fabs(target_deg - current_deg);
        bool in_tolerance = delta_deg <= ANGLE_TOLERANCE_DEG;

        if (feedback != nullptr) feedback[i] = in_tolerance;

        if (in_tolerance) digitalWrite(PIN_LED_FIELD[i], !PIN_LED_FIELD_ACTIVE);
        else digitalWrite(PIN_LED_FIELD[i], PIN_LED_FIELD_ACTIVE);
    }
}
