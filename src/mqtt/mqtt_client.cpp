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

void publishTemperature(float temp, float cjTemp, uint8_t fault) {
    static uint8_t retries = 0;
    const uint8_t MAX_RETRIES = 3;
    
    while (!client.connected() && retries < MAX_RETRIES) {
        Serial.println("[MQTT] Reconnecting before publish...");
        if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
            Serial.println("[MQTT] Reconnected!");
            retries = 0;
            break;
        }
        retries++;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    if (!client.connected()) {
        Serial.println("[MQTT] Cannot publish - not connected!");
        return;
    }
    
    char message[128];
    snprintf(message, sizeof(message), 
             "{\"temp\":%.2f,\"cj\":%.2f,\"fault\":%d,\"time\":%lu}", 
             temp, cjTemp, fault, millis());
    
    if (client.publish(mqtt_topic_temp, message, true)) { // true = retain message
        Serial.printf("[MQTT] Published: %s\n", message);
    } else {
        Serial.println("[MQTT] Publish failed!");
    }
}

void mqttTask(void *pvParameters) {
    while (1) {
        // Ждем сигнала от задачи температуры
        if (xSemaphoreTake(tempSem, portMAX_DELAY) == pdTRUE) {
            publishTemperature(lastTemp, lastCJTemp, lastFault);
        }
        
        if (!client.connected()) {
            Serial.println("[MQTT] Reconnecting...");
            if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
                Serial.println("[MQTT] Reconnected!");
            }
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
        client.loop();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}