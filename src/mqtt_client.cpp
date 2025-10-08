#include "mqtt_client.h"
#include "secrets.h" // Подключаем наши секретные данные
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> // Библиотека для удобной работы с JSON

// Создаем клиенты для WiFi и MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Функция для подключения к Wi-Fi
void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("[MQTT] Connecting to WiFi ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[MQTT] WiFi connected!");
  Serial.print("[MQTT] IP address: ");
  Serial.println(WiFi.localIP());
}

// Функция инициализации MQTT
void setupMqtt() {
  setupWifi(); // Сначала подключаемся к Wi-Fi
  client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  Serial.println("[MQTT] Client configured.");
}

// Функция для переподключения к MQTT брокеру
bool reconnect() {
  Serial.print("[MQTT] Attempting to connect to broker... ");
  // Создаем уникальный ID клиента, чтобы избежать конфликтов
  String clientId = "ESP32-TempSystem-" + String((uint32_t)ESP.getEfuseMac(), HEX);
  
  // Пытаемся подключиться. Указываем LWT (Last Will and Testament) -
  // если ESP32 отвалится, брокер сам отправит в топик статуса "offline".
  if (client.connect(clientId.c_str(), NULL, NULL, MQTT_TOPIC_STATUS, 1, true, "offline")) {
    Serial.println("connected!");
    // После успешного подключения отправляем статус "online"
    client.publish(MQTT_TOPIC_STATUS, "online", true);
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(". Trying again in 5 seconds.");
  }
  return client.connected();
}

// Задача FreeRTOS для работы с MQTT
void mqttTask(void *pvParameters) {
  while (1) { // Бесконечный цикл задачи
    // --- ШАГ 1: ПОДДЕРЖКА СОЕДИНЕНИЯ ---
    if (!client.connected()) {
      if (!reconnect()) {
        vTaskDelay(pdMS_TO_TICKS(5000)); // Если не удалось, ждем 5 секунд
        continue; // и начинаем цикл заново
      }
    }
    client.loop(); // ОБЯЗАТЕЛЬНО вызываем для обработки MQTT сообщений

    // --- ШАГ 2: ОЖИДАНИЕ ДАННЫХ ---
    // Ждем сигнала от задачи датчика. Ждем не вечно, а 1 секунду,
    // чтобы цикл мог продолжаться и обслуживать client.loop().
    if (xSemaphoreTake(tempSem, pdMS_TO_TICKS(1000)) == pdTRUE) {
      
      // --- ШАГ 3: ФОРМИРОВАНИЕ JSON ---
      StaticJsonDocument<256> doc;
      doc["relay_id"] = currentRelay + 1; // Номер реле (1-12)
      doc["temp_c"] = lastTemp;           // Температура термопары
      doc["cj_temp_c"] = lastCJTemp;       // Температура холодного спая
      doc["fault_code"] = lastFault;      // Код ошибки (0 если все ОК)

      // Добавляем расшифровку ошибки, если она есть
      if (lastFault) {
        JsonObject fault_details = doc.createNestedObject("fault_details");
        if (lastFault & 0x01) fault_details["open_circuit"] = true;
        if (lastFault & 0x02) fault_details["over_under_voltage"] = true;
        if (lastFault & 0x04) fault_details["tc_low"] = true;
        if (lastFault & 0x08) fault_details["tc_high"] = true;
        if (lastFault & 0x10) fault_details["cj_low"] = true;
        if (lastFault & 0x20) fault_details["cj_high"] = true;
      }

      String json_output;
      serializeJson(doc, json_output);

      // --- ШАГ 4: ПУБЛИКАЦИЯ ---
      if (client.publish(MQTT_TOPIC_TEMP, json_output.c_str(), true)) { // true = retain
        Serial.println("[MQTT] Published: " + json_output);
      } else {
        Serial.println("[MQTT] Publish failed!");
      }
    }
  }
}