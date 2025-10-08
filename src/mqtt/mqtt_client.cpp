// MQTT и WiFi

#include "mqtt_client.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"

WiFiClient espClient;
PubSubClient client(espClient);

void setupMqtt() {
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[WiFi] Connected to " + String(ssid));

    // MQTT setup - теперь всё должно работать, т.к. типы совпадают
    client.setServer(mqtt_server, mqtt_port);
    
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
        Serial.println("[MQTT] Connected to broker " + String(mqtt_server));
    } else {
        Serial.println("[ERR] MQTT connect failed! rc=" + String(client.state()));
    }
}

void mqttTask(void *pvParameters) {
    while (1) {
        if (!client.connected()) {
            Serial.println("[MQTT] Reconnecting...");
            if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
                Serial.println("[MQTT] Reconnected!");
            }
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
        client.loop();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}