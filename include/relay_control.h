// реле

#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include <Arduino.h>  // Для pinMode, digitalWrite
#include "config.h"   // Для relayPins и констант

void setupRelays();
void relayTask(void *pvParameters);

#endif // RELAY_CONTROL_H