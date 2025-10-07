// инициализация, FreeRTOS задачи

#include <Arduino.h>
#include "relay_control.h"
#include "temp_sensor.h"
#include "secrets.h"
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

SemaphoreHandle_t relaySem = NULL;  // Global semaphore

void setup() {
  Serial.begin(115200);

  setupRelays();
  setupTempSensor();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[WiFi] Connected!");

  // MQTT setup
  client.setServer(mqtt_server, mqtt_port);

  delay(500);

  xTaskCreate(relayTask, "RelayTask", 2048, NULL, 1, NULL);
  xTaskCreate(tempSensorTask, "TempSensorTask", 4096, NULL, 1, NULL);

  Serial.println("[SYSTEM] Setup complete, all relays OFF, tasks started.");
}

void loop() {
  // Empty
}