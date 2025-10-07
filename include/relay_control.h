// реле

#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include <Arduino.h>  // Для pinMode, digitalWrite
#include "config.h"   // Для relayPins и констант

void setupRelays();
void relayTask(void *pvParameters);

// Пины реле (U1-U12) — соответствуют разъёмам термопар 1-12 на корпусе
static const int relayPins[] = {32, 33, 25, 26, 27, 14, 22, 21, 17, 16, 15, 13}; // GPIO пины для реле U1-U12
static const int numRelays = sizeof(relayPins) / sizeof(relayPins[0]); // кол-во реле
static const int invertedRelayIndex = 10; // GPIO15 (U11, P-канальный транзистор управляется минусом, остальные реле подключены напрямую и управляются +3.3V)

// Интервал переключения реле (мс)
static const unsigned long switchInterval = 2000;

#endif // RELAY_CONTROL_H