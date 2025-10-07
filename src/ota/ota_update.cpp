// OTA обновления

#include <Arduino.h>
#include "ota_update.h"

void setupOta() {
  // Будет позже
  Serial.println("[OTA] Setup placeholder");
}

void otaTask(void *pvParameters) {
  while (1) {
    Serial.println("[OTA] Task placeholder");
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}