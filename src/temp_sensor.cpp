// логика MAX31856

#include <Arduino.h>
#include <SPI.h>  // Для SPIClass
#include <Adafruit_MAX31856.h>  // Для maxthermo
#include "temp_sensor.h"
#include "relay_control.h"  // Для relaySem
#include "globals.h"  // Для relaySem

SPIClass *vspi = new SPIClass(VSPI);  // VSPI for MAX31856
Adafruit_MAX31856 maxthermo(MAX_CS);  // CS from config

void setupTempSensor() {
  // Init SPI as in old working code (SCK, MISO, MOSI, CS to match datasheet/Adafruit p1 wiring)
  vspi->begin(MAX_SCK, MAX_MISO, MAX_MOSI, MAX_CS);
  SPI.begin(MAX_SCK, MAX_MISO, MAX_MOSI, MAX_CS);  // Global SPI, matches old to fix 256°C

  if (!maxthermo.begin()) {
    Serial.println("[ERR] MAX31856 not found. Check wiring!");
    while (1);  // Stop if error
  }

  Serial.println("[OK] MAX31856 initialized!");
  maxthermo.setThermocoupleType(THERMOCOUPLE_TYPE);  // K-type from config
}

void tempSensorTask(void *pvParameters) {
  while (1) {
    if (xSemaphoreTake(relaySem, portMAX_DELAY) == pdTRUE) {  // Wait for relay signal (sync)
      // Delay 200ms for stable after relay (ISO7741 delay + B0505S ripple, no garbage)
      vTaskDelay(pdMS_TO_TICKS(200));  // Wait for signal settle (safety from transients)

      // Read temp (thermocouple and cold junction)
      float temp = maxthermo.readThermocoupleTemperature();
      float cjTemp = maxthermo.readCJTemperature();

      Serial.print("[CJ] Cold Junction Temp: ");
      Serial.print(cjTemp);
      Serial.println(" °C");

      Serial.print("[TEMP] Thermocouple Temp: ");
      Serial.print(temp);
      Serial.println(" °C");

      // Full fault check (datasheet p1: detect OV/UV for high voltage KZ)
      uint8_t fault = maxthermo.readFault();
      if (fault) {
        Serial.print("[FAULT] 0x");
        Serial.println(fault, HEX);
        if (fault & MAX31856_FAULT_OPEN) Serial.println("  [!] Open thermocouple");
        if (fault & MAX31856_FAULT_OVUV) Serial.println("  [!] Over/Under voltage");
        if (fault & MAX31856_FAULT_CJHIGH) Serial.println("  [!] CJ temp high");
        if (fault & MAX31856_FAULT_CJLOW) Serial.println("  [!] CJ temp low");
        if (fault & MAX31856_FAULT_TCLOW) Serial.println("  [!] TC temp low");
        if (fault & MAX31856_FAULT_TCHIGH) Serial.println("  [!] TC temp high");
        if (fault & MAX31856_FAULT_CJRANGE) Serial.println("  [!] CJ range error");
      } else {
        Serial.println("[FAULT] No faults detected.");
      }
    }  // Close if (add } here to fix expected '}')
  }  // Close while
}