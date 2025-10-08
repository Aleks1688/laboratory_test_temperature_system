// MQTT/WiFi

#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <Arduino.h>  // Для Serial, String
// #include "secrets.h"  // Для WiFi/MQTT данных

void setupMqtt();  // Инициализация WiFi и MQTT
void mqttTask(void *pvParameters);  // Таск для reconnect и publish

#endif // MQTT_CLIENT_H