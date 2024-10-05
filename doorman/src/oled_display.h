#pragma once
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include "Fonts/Org_01.h"
#include "Fonts/TomThumb.h"
#include "string.h"
#include "platform.h"

#define SSD1306_NO_SPLASH

const uint8_t bitmap_wifi_off[] = {0x80, 0x00, 0x40, 0x00, 0x27, 0x80, 0x31, 0xe0, 0xe8, 0x38, 0x0c, 0x00, 0x3a, 0x60, 0x03, 0x00, 0x0f, 0x80, 0x00, 0xc0, 0x02, 0x20, 0x00, 0x10, 0x00, 0x08};
const uint8_t bitmap_wifi_on[] = {0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x38, 0xe0, 0xc0, 0x18, 0x0f, 0x80, 0x30, 0x60, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define FONT_OFFSET_ORG_01 4
#define FONT_OFFSET_TOMTHUMB 5

const size_t MSG_BUF_SIZE = 4;

struct BusMessage
{
  tm timestamp;         // Timestamp
  char deci_seconds;    // 0.1 second resolution
  int32_t message_data; // 32-bit message (invalid if 0)
  char translation[4];  // 3-character string of the translation
};

class OLEDDisplay
{
private:
  Adafruit_SSD1306 display;

  bool wifi_active = false;
  int wifi_last_ip_octet = 0;
  bool party_mode_active = false;
  tm party_mode_stop_time;
  bool bell_mute_active = false;
  tm bell_mute_stop_time;

  BusMessage message_history_ring_buffer[MSG_BUF_SIZE];
  size_t message_history_ring_buffer_pointer = 0; // Always points to the most recent message

public:
  OLEDDisplay()
  {
    // Initialize I2C
    Wire.setPins(PIN_OLED_SDA, PIN_OLED_SCL); // Set the I2C pins before begin
    Wire.begin();
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    // Initialize message ring buffer
    for (int i = 0; i < MSG_BUF_SIZE; ++i)
    {
      message_history_ring_buffer[0].message_data = 0; // This is a hint for an invalid (empty) message slot
    }
  }

  void init(String startup_text)
  {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
      Serial.println("SSD1306 allocation failed");
      for (;;)
        ;
    }
    else
    {
      display.clearDisplay();
      display.setFont(NULL);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      println(startup_text);
    }
  }

  void println(String text)
  {
    display.println(text);
    display.display();
  }

  void update_party_mode(bool activated, tm stop_timestamp)
  {
    party_mode_active = activated;
    party_mode_stop_time = stop_timestamp;
  }

  void update_bell_mute(bool activated, tm stop_timestamp)
  {
    bell_mute_active = activated;
    bell_mute_stop_time = stop_timestamp;
  }

  void update_wifi(bool active, int last_ip_octet)
  {
    wifi_active = active;
    wifi_last_ip_octet = last_ip_octet;
  }

  void add_message_to_history(tm timeinfo, char deci_seconds, int32_t message_data, const String translation)
  {
    // Update ring buffer
    message_history_ring_buffer_pointer = (message_history_ring_buffer_pointer + 1) % MSG_BUF_SIZE;

    // Write latest message in new position
    message_history_ring_buffer[message_history_ring_buffer_pointer].timestamp = timeinfo;
    message_history_ring_buffer[message_history_ring_buffer_pointer].deci_seconds = deci_seconds;
    message_history_ring_buffer[message_history_ring_buffer_pointer].message_data = message_data;
    if (translation != NULL)
    {
      translation.toCharArray(&message_history_ring_buffer[message_history_ring_buffer_pointer].translation[0], 4);
    }
    else
    {
      message_history_ring_buffer[message_history_ring_buffer_pointer].translation[0] = '\0';
    }
  }

  void draw_buttons()
  {
    if (party_mode_active)
    {
      display.fillRoundRect(0, 0, 43, 13, 2, SSD1306_WHITE);
      display.setFont(&Org_01);
      display.setTextColor(SSD1306_BLACK);
      display.setTextSize(1);
      display.setCursor(7, 1 + FONT_OFFSET_ORG_01);
      display.println("PARTY");
      display.setCursor(7, 7 + FONT_OFFSET_ORG_01);
      display.println("MODE");
      display.setFont(&TomThumb);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(3, 15 + FONT_OFFSET_TOMTHUMB);
      display.printf("Until %02d:%02d", party_mode_stop_time.tm_hour, party_mode_stop_time.tm_min);
    }
    else
    {
      display.fillRect(0, 0, 43, 21, SSD1306_BLACK);
    }

    if (bell_mute_active)
    {
      display.fillRoundRect(55, 0, 43, 13, 2, SSD1306_WHITE);
      display.setFont(&Org_01);
      display.setTextColor(SSD1306_BLACK);
      display.setTextSize(1);
      display.setCursor(66, 1 + FONT_OFFSET_ORG_01);
      display.println("BELL");
      display.setCursor(66, 7 + FONT_OFFSET_ORG_01);
      display.println("MUTE");
      display.setFont(&TomThumb);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(55 + 3, 15 + FONT_OFFSET_TOMTHUMB);
      display.printf("Until %02d:%02d", bell_mute_stop_time.tm_hour, bell_mute_stop_time.tm_min);
    }
    else
    {
      display.fillRect(55, 0, 43, 21, SSD1306_BLACK);
    }
  }

  void draw_wifi()
  {
    if (wifi_active)
    {
      display.drawBitmap(112, 0, bitmap_wifi_on, 13, 13, 1);
      display.setTextColor(SSD1306_WHITE);
      display.setFont(NULL);
      display.setCursor(109, 13);
      char ip_text[4];
      sprintf(ip_text, "%03d", wifi_last_ip_octet);
      display.println(ip_text);
    }
    else
    {
      display.drawBitmap(113, 0, bitmap_wifi_off, 13, 13, 1);
      // TODO: Erase text underneath
    }
  }

  void draw_message_title()
  {
    display.fillRect(0, 22, SCREEN_WIDTH, 7, SSD1306_WHITE);
    display.setFont(&TomThumb);
    display.setTextColor(SSD1306_BLACK);
    display.setTextSize(1);
    display.setCursor(1, 23 + FONT_OFFSET_TOMTHUMB);
    display.println("TC Bus LOG:");
  }

  void draw_message_history()
  {
    for (int i = 0; i < MSG_BUF_SIZE; ++i)
    {
      BusMessage *bus_message = &message_history_ring_buffer[(message_history_ring_buffer_pointer - i) % MSG_BUF_SIZE];
      if (bus_message->message_data == 0)
      {
        break;
      }
      display.setFont(&TomThumb);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 9 * i + 32 + FONT_OFFSET_TOMTHUMB);
      display.printf("%02d:%02d:%02d.%d", bus_message->timestamp.tm_hour, bus_message->timestamp.tm_min, bus_message->timestamp.tm_sec, bus_message->deci_seconds);

      display.setFont(NULL);
      display.setCursor(38, 9 * i + 30);
      display.printf("%04X ", bus_message->message_data >> 16);
      display.printf("%04X", bus_message->message_data & 0x0000FFFF);

      if (bus_message->translation[0] != '\0')
      {
        display.printf(" (%s)", bus_message->translation);
      }
    }
  }

  void redraw()
  {
    display.clearDisplay();
    draw_buttons();
    draw_wifi();
    draw_message_title();
    draw_message_history();
    display.display();
  }
};
