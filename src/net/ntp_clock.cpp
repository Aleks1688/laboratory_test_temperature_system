// NTP синхронизация

#include <Arduino.h>
#include "ntp_clock.h"

void setupNtp() {
  // Будет позже
  Serial.println("[NTP] Setup placeholder");
}

void ntpTask(void *pvParameters) {
  while (1) {
    Serial.println("[NTP] Task placeholder");
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}