// логика переключения реле

#include <Arduino.h>
#include "relay_control.h"
#include "globals.h"  // Для relaySem

void setupRelays() {
  // Настраиваем все пины как выходы и ставим в выкл состояние
  for (int i = 0; i < numRelays; i++) {
    pinMode(relayPins[i], OUTPUT);  // Устанавливаем пин как выход (GPIO out)
    if (i == invertedRelayIndex) {
      digitalWrite(relayPins[i], HIGH); // Для U11 (инвертированного): HIGH = выкл (+3.3V, канал открыт, нагрузка off)
    } else {
      digitalWrite(relayPins[i], LOW);  // Для остальных (U1-U10, U12): LOW = выкл (0V, канал закрыт, нагрузка off)
    }
  }
  // Логируем для дебага: все реле выкл при старте
  Serial.println("[RELAY] All relays initialized to OFF state.");
}

void relayTask(void *pvParameters) {
  static int currentRelay = -1;  // Начинаем с -1, чтобы первое переключение прошло на U1

  while (1) {  // Бесконечный цикл задачи FreeRTOS
    // Шаг 1: Выключаем все реле (гарантия, что только одно вкл, нет КЗ)
    for (int i = 0; i < numRelays; i++) {
      if (i == invertedRelayIndex) {
        digitalWrite(relayPins[i], HIGH); // Выкл U11 (+3.3V)
      } else {
        digitalWrite(relayPins[i], LOW);  // Выкл остальные (0V)
      }
    }
    Serial.println("[RELAY] All relays turned OFF for discharge.");

    // Шаг 2: Задержка на разрядку реле и нагрузки (CPC2014N off-time ~1ms, но для термопар под напряжением даём 100ms на discharge)
    vTaskDelay(pdMS_TO_TICKS(100));  // 100мс — безопасный запас, чтобы заряд ушёл, нет transients (по даташиту CPC2014N и ISO7741)

    // Шаг 3: Переходим к следующему реле (циклически 0-11)
    currentRelay = (currentRelay + 1) % numRelays;

    // Шаг 4: Включаем текущее реле
    if (currentRelay == invertedRelayIndex) {
      digitalWrite(relayPins[currentRelay], LOW); // Вкл U11 (0V)
    } else {
      digitalWrite(relayPins[currentRelay], HIGH); // Вкл остальные (+3.3V)
    }

    // Шаг 5: Логирование для дебага (чтобы видеть, что вкл и когда)
    Serial.print("[RELAY] ON → U");
    Serial.print(currentRelay + 1);  // +1 для человеческого счёта U1-U12
    Serial.print(" (GPIO");
    Serial.print(relayPins[currentRelay]);
    Serial.println(")");
    
    vTaskDelay(pdMS_TO_TICKS(200));  // Delay after on for stable before signal temp
    xSemaphoreGive(relaySem);  // Give signal to temp task (release semaphore)

    // Шаг 6: Ждём интервал для следующего переключения (switchInterval минус разрядка, чтобы итого 2000ms)
    vTaskDelay(pdMS_TO_TICKS(switchInterval - 300));  // Не грузит CPU, FreeRTOS scheduler работает
  }
}