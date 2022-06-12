#ifndef SPECIFICS_H
#define SPECIFICS_H

#define STR_(X) #X
#define STR(X)  STR_(X)

// CHANGE THESE ONLY (and specifics.cpp)
#define BOARD_ID 0
#define LIGHT_COUNT 3
#define MAIN_SERVER_ADDR "192.168.1.100"

#include <Arduino.h>

extern const byte buttonPins[LIGHT_COUNT];
extern const byte lightPins[LIGHT_COUNT];
extern const String lightIDs[LIGHT_COUNT];

extern const String serverAddress;

extern bool states[LIGHT_COUNT];

extern void (*ISRlist[LIGHT_COUNT])();

#endif
