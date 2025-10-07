// SPIFFS/NVS для настроек

#include <Arduino.h>
#include "flash_store.h"

void setupStorage() {
  // Будет позже
  Serial.println("[STORAGE] Setup placeholder");
}

void saveData(const char* key, const char* value) {
  // Будет позже
}

String loadData(const char* key) {
  return ""; // Будет позже
}