#include "flight_stick_comms.hpp"
#include <functional>
#include <Arduino.h>
#include <Wire.h>

volatile struct FlightStickState _state; // State to relay when requested
TwoWire* _bus;

void handle_flight_stick_request() {
    _bus->write((uint8_t*) &_state, sizeof(struct FlightStickState));
}

void setup_to_serve(TwoWire* bus, uint8_t address) {
    _bus = bus;
    _bus->onRequest(handle_flight_stick_request);
}

void setup_to_request(TwoWire* bus) {
    _bus = bus;
}

void read_complete(uint8_t address, struct FlightStickState* destination) {
    const uint8_t REQUEST_SIZE = sizeof(struct FlightStickState);

    _bus->requestFrom(address, REQUEST_SIZE);

    if (_bus->available() != REQUEST_SIZE) return;
    
    _bus->readBytes((uint8_t*) destination, REQUEST_SIZE); 
}

void update_output(struct FlightStickState* source) {
    noInterrupts();
    memcpy((void *)&_state, source, sizeof(struct FlightStickState));
    interrupts();
}