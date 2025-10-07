// SPIFFS/NVS

#ifndef FLASH_STORE_H
#define FLASH_STORE_H

#include <Arduino.h>

void setupStorage(); // Initialize SPIFFS or NVS
void saveData(const char* key, const char* value); 
String loadData(const char* key); 

#endif // FLASH_STORE_H