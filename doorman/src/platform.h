#pragma once
#include <Arduino.h>

#define VERSION "2024.4.1"

#ifdef ESP8266
#define PIN_BUS_READ D5
#define PIN_BUS_WRITE D6
#define SYSTEM_NAME "ESP8266 Doorman"
#endif

#ifdef ESP32
#define SYSTEM_NAME "ESP32 Doorman"
#endif

#ifdef ARDUINO_ESP32C3_DEV
#define PIN_BUS_READ 2
#define PIN_BUS_WRITE 1
#define PIN_OLED_SDA 9
#define PIN_OLED_SCL 10
#define LED_WHITE 19
#endif