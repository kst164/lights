#include "specifics.h"
#include "ISRs.h"

ICACHE_RAM_ATTR void buttonISR0() {
    triggers[0] = true;
}

ICACHE_RAM_ATTR void buttonISR1() {
    triggers[1] = true;
}

ICACHE_RAM_ATTR void buttonISR2() {
    triggers[2] = true;
}

ICACHE_RAM_ATTR void buttonISR3() {
    triggers[3] = true;
}

ICACHE_RAM_ATTR void buttonISR4() {
    triggers[4] = true;
}

volatile bool triggers[LIGHT_COUNT];
