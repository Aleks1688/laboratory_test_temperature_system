// данный файл описывает пины, константы

#ifndef CONFIG_H
#define CONFIG_H

// Пины реле (U1-U12) — соответствуют разъёмам термопар 1-12 на корпусе
static const int relayPins[] = {32, 33, 25, 26, 27, 14, 22, 21, 17, 16, 15, 13};
static const int numRelays = sizeof(relayPins) / sizeof(relayPins[0]);
static const int invertedRelayIndex = 10; // GPIO15 (U11, P-канальный транзистор управляется минусом, остальные реле подключены напрямую и управляются +3.3V)

// Пины MAX31856 (SPI через ISO7741, исправленная последовательность)
#define MAX_CS   5
#define MAX_MOSI 18
#define MAX_MISO 19
#define MAX_SCK  23


// Интервал переключения реле (мс)
static const unsigned long switchInterval = 2000;

// Thermocouple type for MAX31856 (K-type as in old code, from datasheet linearization)
#define THERMOCOUPLE_TYPE MAX31856_TCTYPE_K

// Semaphore for sync relay to temp (global, null init)
extern SemaphoreHandle_t relaySem;

#endif // CONFIG_H