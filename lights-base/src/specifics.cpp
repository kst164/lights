#include "specifics.h"
#include "ISRs.h"

const byte buttonPins[LIGHT_COUNT] = {D5, D6, D7};
const byte lightPins[LIGHT_COUNT] = {D1, D2, D3};
const String lightIDs[LIGHT_COUNT] = {"1", "2", "3"};
bool states[LIGHT_COUNT] = {LOW, LOW, LOW};


void (*ISRlist[LIGHT_COUNT])() = {buttonISR0, buttonISR1, buttonISR2};
