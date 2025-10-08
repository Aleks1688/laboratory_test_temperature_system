// логика переключения реле

#include "relay_control.h"
#include "config.h" // Подключаем наш файл с настройками пинов и интервалов

// Функция инициализации. Настраивает пины реле.
void setupRelays() {
  for (int i = 0; i < NUM_RELAYS; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
    // Сразу устанавливаем реле в ВЫКЛЮЧЕННОЕ состояние
    // Учитываем, что одно реле имеет инвертированную логику
    if (i == INVERTED_RELAY_INDEX) {
      digitalWrite(RELAY_PINS[i], HIGH); // Для инвертированного HIGH = ВЫКЛ
    } else {
      digitalWrite(RELAY_PINS[i], LOW); // Для обычного LOW = ВЫКЛ
    }
  }
  Serial.println("[RELAY] All relays initialized to OFF state.");
}

// Задача FreeRTOS для управления реле
void relayTask(void *pvParameters) {
  // Начинаем с последнего реле, чтобы цикл в первый раз выбрал реле с индексом 0
  currentRelay = NUM_RELAYS - 1;

  while (1) { // Бесконечный цикл задачи
    // --- ШАГ 1: БЕЗОПАСНОСТЬ - ВЫКЛЮЧИТЬ ВСЕ РЕЛЕ ---
    for (int i = 0; i < NUM_RELAYS; i++) {
      if (i == INVERTED_RELAY_INDEX) {
        digitalWrite(RELAY_PINS[i], HIGH);
      } else {
        digitalWrite(RELAY_PINS[i], LOW);
      }
    }
    
    // --- ШАГ 2: БЕЗОПАСНОСТЬ - ПАУЗА НА РАЗРЯДКУ ---
    // Даем время, чтобы контакты разомкнулись и напряжение спало.
    // Это самый важный шаг для предотвращения КЗ!
    vTaskDelay(pdMS_TO_TICKS(RELAY_DISCHARGE_DELAY_MS));

    // --- ШАГ 3: ВЫБОР СЛЕДУЮЩЕГО РЕЛЕ ---
    currentRelay = (currentRelay + 1) % NUM_RELAYS; // Циклический переход (0, 1, ..., 11, 0, 1, ...)

    // --- ШАГ 4: ВКЛЮЧЕНИЕ ОДНОГО РЕЛЕ ---
    Serial.printf("[RELAY] Switching to relay U%d (GPIO %d)...\n", currentRelay + 1, RELAY_PINS[currentRelay]);
    if (currentRelay == INVERTED_RELAY_INDEX) {
      digitalWrite(RELAY_PINS[currentRelay], LOW); // Для инвертированного LOW = ВКЛ
    } else {
      digitalWrite(RELAY_PINS[currentRelay], HIGH); // Для обычного HIGH = ВКЛ
    }

    // --- ШАГ 5: СИГНАЛ ДЛЯ СЛЕДУЮЩЕЙ ЗАДАЧИ ---
    // Говорим задаче датчика, что можно начинать измерение
    xSemaphoreGive(relaySem);

    // --- ШАГ 6: ОЖИДАНИЕ ---
    // Ждем оставшееся время цикла перед следующим переключением
    vTaskDelay(pdMS_TO_TICKS(RELAY_CYCLE_INTERVAL_MS));
  }
}