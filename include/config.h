// данный файл описывает пины, константы

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>  // Для uint8_t etc.

// Semaphore for sync (relay to temp to mqtt)
extern SemaphoreHandle_t relaySem;  // Семафор от реле к датчику (сигнал "переключил, читай")
extern SemaphoreHandle_t tempSem;  // Семафор от датчика к mqtt (сигнал "прочитал, шлёшь")

#endif // CONFIG_H