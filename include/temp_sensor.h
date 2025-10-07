// MAX31856

#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <Arduino.h>  // Для Serial, float
#include "config.h"   // Для MAX_CS, MAX_MOSI etc.

void setupTempSensor();
void tempSensorTask(void *pvParameters);

#endif // TEMP_SENSOR_H