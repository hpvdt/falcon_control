#ifndef _FLIGHT_STICK_COMMS_H
#define _FLIGHT_STICK_COMMS_H

#include <Arduino.h>
#include <Wire.h>
#include "flight_stick_state.h"

void fs_setup_to_serve(TwoWire* bus, uint8_t address);
void fs_setup_to_request(TwoWire* bus);
void fs_read_complete(uint8_t address, struct FlightStickState* destination);
void fs_update_output(struct FlightStickState* source);

#endif