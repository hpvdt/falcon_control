#include "pin_allocations.hpp"

const int PIN_SERVOS[5] = {PIN_SERVO_1, PIN_SERVO_2, PIN_SERVO_3, PIN_SERVO_4, PIN_SERVO_5}; // Servos ordered from port to broadside, then ESC
const int PIN_POTENTIOMETERS[4] = {PIN_POTENTIOMETER_1, PIN_POTENTIOMETER_2, PIN_POTENTIOMETER_3, PIN_POTENTIOMETER_4}; // Control surface feedback potentiometers, from port to broadside
const int PIN_LED_FIELD[4] = {PIN_LED_FIELD_1, PIN_LED_FIELD_2, PIN_LED_FIELD_3, PIN_LED_FIELD_4}; // LEDs reporting surface state, from port to broadside