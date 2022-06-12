#ifndef ISRS_H
#define ISRS_H

#include "Arduino.h"
#include "specifics.h"

void buttonISR0();
void buttonISR1();
void buttonISR2();
void buttonISR3();
void buttonISR4();

extern volatile bool triggers[LIGHT_COUNT];

#endif
