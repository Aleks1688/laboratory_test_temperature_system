// MAX31856

#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <Arduino.h> // Нужен для стандартных типов и FreeRTOS

// --- Объявления для доступа из других файлов ---

// Семафоры для синхронизации
extern SemaphoreHandle_t relaySem; // Этот семафор мы "берем"
extern SemaphoreHandle_t tempSem;  // Этот семафор мы "отдаем"

// Глобальные переменные для хранения последнего измерения.
// Модуль MQTT будет их читать.
extern float lastTemp;
extern float lastCJTemp;
extern uint8_t lastFault;


// --- Прототипы функций ---

// Функция инициализации MAX31856. Вызывается один раз в setup().
void setupTempSensor();

// Функция задачи FreeRTOS. Будет работать в бесконечном цикле.
void tempSensorTask(void *pvParameters);

#endif // TEMP_SENSOR_H