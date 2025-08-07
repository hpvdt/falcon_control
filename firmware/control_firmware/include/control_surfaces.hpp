#ifndef CONTROL_SURFACES_H
#define CONTROL_SURFACES_H

#include <Arduino.h>

enum ControlSurface {
    CTRL_SRFC_LFT_TIP   = 0,
    CTRL_SRFC_RDR       = 1,
    CTRL_SRFC_ELVTR     = 2,
    CTRL_SRFC_RGHT_TIP  = 3,
    CTRL_SRFC_COUNT
};

void ctrl_srfc_init(Stream* debug);

void ctrl_srfc_set(enum ControlSurface surface, float angle);

float ctrl_srfc_read_surface(enum ControlSurface query);

void ctrl_srfc_check_state_all(bool feedback[] = nullptr);

#endif