#pragma once
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include "Fonts/Org_01.h"
#include "Fonts/TomThumb.h"


const uint8_t bitmap_wifi_off[] = {0x80, 0x00, 0x40, 0x00, 0x27, 0x80, 0x31, 0xe0, 0xe8, 0x38, 0x0c, 0x00, 0x3a, 0x60, 0x03, 0x00, 0x0f, 0x80, 0x00, 0xc0, 0x02, 0x20, 0x00, 0x10, 0x00, 0x08};
const uint8_t bitmap_wifi_on[] = {0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x38, 0xe0, 0xc0, 0x18, 0x0f, 0x80, 0x30, 0x60, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

class DisplayHelper {
private:
    Adafruit_SSD1306 display;

public:
    DisplayHelper() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

    void init() {
        if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
            Serial.println(F("SSD1306 allocation failed"));
            for (;;);
        } else {
          Serial.println(F("Successfully initialized display"));
        }
    }

    void oled_draw_wifi(bool active, int last_ip_octet) {
      display.clearDisplay();
      display.setTextSize(2);      // Set text size
      display.setTextColor(SSD1306_WHITE); // Set text color
      display.setCursor(0, 10);    // Set position to start printing text
      display.println("Hello");
      display.print("World");

      if (active) {
        display.drawBitmap(98, 0, bitmap_wifi_on, 13, 13, 1);
        display.setCursor(111, 13);
        display.setTextColor(SSD1306_WHITE);
        display.setFont(NULL);
        char ip_text[4];
        sprintf(ip_text, "%0d3", last_ip_octet);
        display.println(ip_text);
      }
      else {
        display.drawBitmap(113, 0, bitmap_wifi_off, 13, 13, 1);
      }
      display.display();
    }
};
