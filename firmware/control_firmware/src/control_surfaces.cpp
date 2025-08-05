#include <Arduino.h>
#include <Servo.h>
#include "pin_allocations.hpp"

#include "control_surfaces.hpp"


Servo actuator[NUM_SERVOS];
const int ACTUATOR_DEFAULT = 90;
const int ACTUATOR_MINIMUM = 90;
const int ACTUATOR_MAXIMUM = 90;

void control_surfaces_setup(Stream* debug) {
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
    for (int i = 0; i < NUM_SERVOS; i++) actuator[i].write(ACTUATOR_MINIMUM);
    delay(1000);
    for (int i = 0; i < NUM_SERVOS; i++) actuator[i].write(ACTUATOR_MAXIMUM);
    delay(1000);
    for (int i = 0; i < NUM_SERVOS; i++) actuator[i].write(ACTUATOR_DEFAULT);
    delay(1000);
    if (debug != nullptr) debug->println(F("Control surfaces completed sweep"));
}
