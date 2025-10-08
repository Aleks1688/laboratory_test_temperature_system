#include <Arduino.h>
#include "config.h"
#include "relay_control.h"
#include "temp_sensor.h"
#include "mqtt_client.h"

// Глобальные семафоры для синхронизации
SemaphoreHandle_t relaySem = NULL;
SemaphoreHandle_t tempSem = NULL;

// Глобальные переменные для обмена данными между задачами
uint8_t currentRelay = 0;
float lastTemp = 0.0f;
float lastCJTemp = 0.0f;
uint8_t lastFault = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\n[SYSTEM] Booting...");

  // Создаем семафоры
  relaySem = xSemaphoreCreateBinary();
  tempSem = xSemaphoreCreateBinary();

  if (relaySem == NULL || tempSem == NULL) {
    Serial.println("[FATAL] Could not create semaphores. Halting.");
    while (1);
  }
  Serial.println("[SYSTEM] Semaphores created.");

  // Инициализация модулей в правильном порядке
  setupRelays();
  setupTempSensor();
  setupMqtt();

  // Создание задач FreeRTOS
  // Задаче датчика даем приоритет повыше, чтобы она выполнялась сразу после сигнала
  xTaskCreate(relayTask, "RelayTask", 2048, NULL, 1, NULL);
  xTaskCreate(tempSensorTask, "TempSensorTask", 4096, NULL, 2, NULL);
  xTaskCreate(mqttTask, "MqttTask", 4096, NULL, 1, NULL);

  Serial.println("[SYSTEM] Setup complete. Tasks are running.");
}

void loop() {
  // Пусто. Вся логика в задачах FreeRTOS.
  // Удаляем задачу loop(), чтобы она не тратила ресурсы процессора.
  vTaskDelete(NULL);
}