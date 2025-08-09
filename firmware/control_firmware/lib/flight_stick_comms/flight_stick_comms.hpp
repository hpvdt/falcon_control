#ifndef _FLIGHT_STICK_COMMS_H
#define _FLIGHT_STICK_COMMS_H

#include <Arduino.h>
#include <Wire.h>

typedef int16_t FlightStickAxis; // Representation used for each axis 
// Signed 16-bit is standard for game controllers, should be enough for us

struct FlightStickButtons {
    bool trim_pitch_up : 1;
    bool trim_pitch_down : 1;
    bool trim_roll_up : 1;
    bool trim_roll_down : 1;
    bool trim_yaw_up : 1;
    bool trim_yaw_down : 1;
    bool hold_heading : 1;
    bool seat_ejection : 1;
};

struct FlightStickState {
    FlightStickAxis axis_roll;
    FlightStickAxis axis_pitch;
    FlightStickAxis axis_yaw;

    // Use a `union` to have these share the same space in memory
    union {
        uint32_t button_mask;               // Summary of all buttons
        struct FlightStickButtons button;   // Pull specific buttons
    };
};

void fs_setup_to_serve(TwoWire* bus, uint8_t address);
void fs_setup_to_request(TwoWire* bus);
void fs_read_complete(uint8_t address, struct FlightStickState* destination);
void fs_update_output(struct FlightStickState* source);

#endif