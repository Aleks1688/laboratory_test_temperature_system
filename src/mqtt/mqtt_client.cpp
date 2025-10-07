// MQTT и WiFi

#include <Arduino.h>
#include "mqtt_client.h"

void setupMqtt() {
  // Будет позже
  Serial.println("[MQTT] Setup placeholder");
}

void mqttTask(void *pvParameters) {
  while (1) {
    Serial.println("[MQTT] Task placeholder");
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}