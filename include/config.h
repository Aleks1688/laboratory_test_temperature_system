// данный файл описывает пины, константы

#ifndef CONFIG_H
#define CONFIG_H

// --- НАСТРОЙКИ SPI ДЛЯ MAX31856 ---
// Важно! Ты указал MOSI=18, SCK=23. Это пины HSPI.
// Стандартный VSPI использует MOSI=23, SCK=18. Оставляю как у тебя.
#define MAX_CS   5
#define MAX_MOSI 18
#define MAX_MISO 19
#define MAX_SCK  23

// --- НАСТРОЙКИ РЕЛЕ ---
// Пины для реле U1-U12
const int RELAY_PINS[] = {32, 33, 25, 26, 27, 14, 22, 21, 17, 16, 15, 13};
const int NUM_RELAYS = sizeof(RELAY_PINS) / sizeof(RELAY_PINS[0]);

// Номер реле с инвертированной логикой (P-канал).
// В массиве он 11-й, его индекс 10.
// Ты писал про GPIO12, но в конфиге был GPIO15 (U11). Оставляю U11 (индекс 10).
const int INVERTED_RELAY_INDEX = 10;

// --- НАСТРОЙКИ ДАТЧИКА ---
// Тип термопары (K, J, T, и т.д.)
#define THERMOCOUPLE_TYPE MAX31856_TCTYPE_K // K-type thermocouple

// --- ВРЕМЕННЫЕ ИНТЕРВАЛЫ (в миллисекундах) ---
// Пауза между выключением всех реле и включением следующего.
// Критически важно для предотвращения КЗ!
const int RELAY_DISCHARGE_DELAY_MS = 200;

// Общее время цикла для одного реле (измерение + отправка).
// Например, 5 секунд на каждую термопару.
const int RELAY_CYCLE_INTERVAL_MS = 5000;

// Пауза после переключения реле перед чтением температуры для стабилизации.
const int TEMP_STABILIZE_DELAY_MS = 100;

#endif // CONFIG_H