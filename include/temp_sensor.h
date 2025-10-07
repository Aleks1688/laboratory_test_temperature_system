// MAX31856

#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <Arduino.h>  // Для Serial, float
#include "config.h"   // Для MAX_CS, MAX_MOSI etc.

void setupTempSensor();
void tempSensorTask(void *pvParameters);

// Пины MAX31856 (SPI через ISO7741, исправленная последовательность)
#define MAX_CS   5
#define MAX_MOSI 18
#define MAX_MISO 19
#define MAX_SCK  23

// Thermocouple type for MAX31856 (K-type as in old code, from datasheet linearization)
#define THERMOCOUPLE_TYPE MAX31856_TCTYPE_K // K-type thermocouple

#endif // TEMP_SENSOR_H