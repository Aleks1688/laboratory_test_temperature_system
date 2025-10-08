// MQTT/WiFi

#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <Arduino.h>  // Для Serial, String


void setupMqtt();  // Инициализация WiFi и MQTT
void mqttTask(void *pvParameters);  // Таск для reconnect и publish

// Объявляем внешние переменные
extern float lastTemp;
extern float lastCJTemp;
extern uint8_t lastFault;
extern SemaphoreHandle_t tempSem;

void setupMqtt();
void mqttTask(void *pvParameters);
void publishTemperature(float temp, float cjTemp, uint8_t fault);

#endif // MQTT_CLIENT_H