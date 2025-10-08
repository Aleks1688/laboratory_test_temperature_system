// данный файл описывает пины, константы

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>  // Для uint8_t etc.

// Semaphore for sync (relay to temp to mqtt)
extern SemaphoreHandle_t relaySem;  // Already for relay to temp
extern SemaphoreHandle_t tempSem;  // New for temp to mqtt

#endif // CONFIG_H