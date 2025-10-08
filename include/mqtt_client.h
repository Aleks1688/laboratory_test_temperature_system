// MQTT/WiFi


#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <Arduino.h> // Нужен для стандартных типов и FreeRTOS

// --- Объявления для доступа из других файлов ---

// Семафор, который мы "берем"
extern SemaphoreHandle_t tempSem;

// Глобальные переменные, которые мы будем читать
extern uint8_t currentRelay;
extern float lastTemp;
extern float lastCJTemp;
extern uint8_t lastFault;


// --- Прототипы функций ---

// Функция инициализации Wi-Fi и MQTT. Вызывается один раз в setup().
void setupMqtt();

// Функция задачи FreeRTOS. Будет работать в бесконечном цикле.
void mqttTask(void *pvParameters);

#endif // MQTT_CLIENT_H