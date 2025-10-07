// инициализация, FreeRTOS задачи

#include <Arduino.h>
#include "relay_control.h"
#include "temp_sensor.h"

SemaphoreHandle_t relaySem = NULL;  // Global semaphore

void setup() {
  Serial.begin(115200);

  setupRelays();  // All-off first
  setupTempSensor();  // Init MAX after relays

  delay(500);  // Stabilize

relaySem = xSemaphoreCreateBinary();  // Create binary semaphore (0/1, starts 0)
if (relaySem == NULL) {
  Serial.println("[ERR] Semaphore create failed!");
  while (1);  // Stop if error
}

  xTaskCreate(relayTask, "RelayTask", 2048, NULL, 1, NULL);
  xTaskCreate(tempSensorTask, "TempSensorTask", 4096, NULL, 1, NULL);  // Bigger stack for lib

  Serial.println("[SYSTEM] Setup complete, all relays OFF, tasks started.");
}

void loop() {
  // Empty
}