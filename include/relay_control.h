// реле

#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include <Arduino.h> // Нужен для стандартных типов и FreeRTOS

// --- Объявления для доступа из других файлов ---

// Семафор, который эта задача будет "отдавать" после переключения реле.
// Задаче датчика нужно будет его "взять".
extern SemaphoreHandle_t relaySem;

// Глобальная переменная, хранящая номер текущего активного реле (0-11).
// Другие модули (например, MQTT) будут ее читать.
extern uint8_t currentRelay;


// --- Прототипы функций ---

// Функция инициализации пинов реле. Вызывается один раз в setup().
void setupRelays();

// Функция задачи FreeRTOS. Будет работать в бесконечном цикле.
void relayTask(void *pvParameters);

#endif // RELAY_CONTROL_H