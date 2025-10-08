// инициализация, FreeRTOS задачи

#include <Arduino.h>
#include "relay_control.h"
#include "temp_sensor.h"
#include "mqtt_client.h"
#include "globals.h"


void setup() {
  Serial.begin(115200);

    // создаём семафоры здесь
  relaySem = xSemaphoreCreateBinary();
  if (relaySem == NULL) {
    Serial.println("[ERR] relaySem failed");
    while (1);
  }
  tempSem = xSemaphoreCreateBinary();
  if (tempSem == NULL) {
    Serial.println("[ERR] tempSem failed");
    while (1);
  }

  setupRelays(); // Init relays
  setupTempSensor(); // Init MAX31856
  setupMqtt();  // Init WiFi/MQTT
  
  delay(500);

  xTaskCreate(relayTask, "RelayTask", 2048, NULL, 1, NULL); // Task for relay switching
  xTaskCreate(tempSensorTask, "TempSensorTask", 4096, NULL, 1, NULL); // Task for reading temp
  xTaskCreate(mqttTask, "MqttTask", 4096, NULL, 1, NULL);  // Task for MQTT

  Serial.println("[SYSTEM] Setup complete, all relays OFF, tasks started.");
}

void loop() {
  // Empty
}