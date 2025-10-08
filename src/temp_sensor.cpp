#include "temp_sensor.h"
#include "config.h" // Подключаем наш файл с настройками
#include <Adafruit_MAX31856.h> // Библиотека для работы с датчиком

// Создаем объект для работы с датчиком, используя пин CS из конфига
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(MAX_CS);

// Функция инициализации датчика
void setupTempSensor() {
  // Запускаем SPI. Важно! Библиотека Adafruit использует глобальный объект SPI.
  // Мы не создаем новый SPIClass, а настраиваем стандартный.
  SPI.begin(MAX_SCK, MAX_MISO, MAX_MOSI, MAX_CS);

  if (!maxthermo.begin()) {
    Serial.println("[TEMP SENSOR] FATAL: MAX31856 not found. Check wiring!");
    Serial.println("[SYSTEM] Halting.");
    while (1); // Остановка системы, если датчик не найден
  }

  maxthermo.setThermocoupleType(THERMOCOUPLE_TYPE);
  Serial.println("[TEMP SENSOR] MAX31856 initialized successfully.");
  Serial.print("[TEMP SENSOR] Thermocouple type set to: ");
  
  // Выводим тип термопары для отладки
  switch (maxthermo.getThermocoupleType()) {
    case MAX31856_TCTYPE_B: Serial.println("B"); break;
    case MAX31856_TCTYPE_E: Serial.println("E"); break;
    case MAX31856_TCTYPE_J: Serial.println("J"); break;
    case MAX31856_TCTYPE_K: Serial.println("K"); break;
    case MAX31856_TCTYPE_N: Serial.println("N"); break;
    case MAX31856_TCTYPE_R: Serial.println("R"); break;
    case MAX31856_TCTYPE_S: Serial.println("S"); break;
    case MAX31856_TCTYPE_T: Serial.println("T"); break;
    default: Serial.println("Unknown"); break;
  }
}

// Задача FreeRTOS для измерения температуры
void tempSensorTask(void *pvParameters) {
  while (1) { // Бесконечный цикл задачи
    // --- ШАГ 1: ОЖИДАНИЕ СИГНАЛА ---
    // Задача "засыпает" здесь и ждет, пока relayTask не даст семафор.
    // portMAX_DELAY означает ждать вечно.
    if (xSemaphoreTake(relaySem, portMAX_DELAY) == pdTRUE) {
      
      // --- ШАГ 2: ПАУЗА НА СТАБИЛИЗАЦИЮ ---
      // Небольшая задержка после переключения реле, чтобы сигнал термопары стабилизировался.
      vTaskDelay(pdMS_TO_TICKS(TEMP_STABILIZE_DELAY_MS));

      // --- ШАГ 3: ЧТЕНИЕ ДАННЫХ ---
      // Считываем все данные и сохраняем их в глобальные переменные.
      lastTemp = maxthermo.readThermocoupleTemperature();
      lastCJTemp = maxthermo.readCJTemperature();
      lastFault = maxthermo.readFault();

      Serial.printf("[TEMP SENSOR] Read data: TC=%.2f C, CJ=%.2f C, Fault=0x%X\n", lastTemp, lastCJTemp, lastFault);

      // --- ШАГ 4: СИГНАЛ ДЛЯ СЛЕДУЮЩЕЙ ЗАДАЧИ ---
      // Говорим задаче MQTT, что данные готовы и можно их отправлять.
      xSemaphoreGive(tempSem);
    }
  }
}