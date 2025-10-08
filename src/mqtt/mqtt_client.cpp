// MQTT и WiFi

#include "mqtt_client.h"
#include <WiFi.h>  // Для WiFi.begin
#include <PubSubClient.h>  // Для MQTT
#include "secrets.h" // Для ssid/pass и mqtt данные

WiFiClient espClient;  // WiFi клиент for MQTT
PubSubClient client(espClient);  // MQTT клиент

void setupMqtt() {
  // Connect to WiFi from secrets.h
  WiFi.begin(ssid, password);  // ssid/pass from secrets.h
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);  // Wait for connect, 500ms per try
    Serial.print(".");
  }
  Serial.println("\n[WiFi] Connected to " + String(ssid));

  // MQTT setup
  client.setServer(mqtt_server, mqtt_port);  // broker/port from secrets.h
  client.connect("ESP32Client", mqtt_user, mqtt_pass);  // Connect with user/pass, ID "ESP32Client"
  if (client.connected()) {
    Serial.println("[MQTT] Connected to broker " + String(mqtt_server));
  } else {
    Serial.println("[ERR] MQTT connect failed! Check broker/user/pass.");
  }
}

void mqttTask(void *pvParameters) {
  while (1) {
    // Reconnect if lost (industrial reliability: check every 5sec)
    if (!client.connected()) {
      Serial.println("[MQTT] Reconnecting...");
      client.connect("ESP32Client", mqtt_user, mqtt_pass);
      vTaskDelay(pdMS_TO_TICKS(5000));  // Wait 5sec before retry
    } else {
      client.loop();  // Keep MQTT alive (subscribe/publish if needed)
    }

    // Publish data here later (temp, relay etc. in JSON)
    // e.g., client.publish("topic", "data");

    vTaskDelay(pdMS_TO_TICKS(1000));  // Check every 1sec
  }
}