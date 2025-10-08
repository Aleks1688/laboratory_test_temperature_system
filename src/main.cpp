// инициализация, FreeRTOS задачи

#include <Arduino.h>
#include "relay_control.h"
#include "temp_sensor.h"
#include "mqtt_client.h"
#include "config.h"

SemaphoreHandle_t relaySem = NULL;  // Глобальный семафор relay -> temp
SemaphoreHandle_t tempSem = NULL;  // Глобальный семафор temp -> mqtt

void setup() {
  Serial.begin(115200);

// Создаём семафоры для sync (binary — 0/1, старт 0 = жди)
relaySem = xSemaphoreCreateBinary();
tempSem = xSemaphoreCreateBinary();

if (relaySem == NULL || tempSem == NULL) {
  Serial.println("[ERR] Семафоры не созданы! Остановка.");
  while (1);  // Stop if error, safety
}
Serial.println("[SYNC] Семафоры созданы для sync задач.");


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