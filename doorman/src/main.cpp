#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <time.h>

#include <oled_display.h>

OLEDDisplay oled_display;

time_t target_time = 0;

void setup()
{
  // Set up serial port
  Serial.begin(115200);

  // Set up WiFi
  oled_display.init("Starting WiFi...");
  Serial.print("\nConnecting to WiFi...");
  WiFi.begin("Cyrah", "MornamNueni?");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  Serial.println(" Success.");

  // Synchronize RTC time with NTP server
  oled_display.println("Synchronizing time...");
  Serial.print("Syncing time with NTP server...");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo))
  {
    configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "ch.pool.ntp.org");
    delay(500);
  }
  Serial.println(" Success.");

  // Finish stuff
  oled_display.println("Startup complete.");
  delay(1000);

  // Add 5 dummy message entries
  struct timeval tv_now;
  getLocalTime(&timeinfo);
  gettimeofday(&tv_now, NULL);
  oled_display.add_message_to_history(timeinfo, (tv_now.tv_usec + 5e4) / 1e5, 0x3f930029, "DBE");
  getLocalTime(&timeinfo);
  gettimeofday(&tv_now, NULL);
  oled_display.add_message_to_history(timeinfo, (tv_now.tv_usec + 5e4) / 1e5, 0x3f930030, "DBF");
  getLocalTime(&timeinfo);
  gettimeofday(&tv_now, NULL);
  oled_display.add_message_to_history(timeinfo, (tv_now.tv_usec + 5e4) / 1e5, 0x3f930031, "DBG");
  getLocalTime(&timeinfo);
  gettimeofday(&tv_now, NULL);
  oled_display.add_message_to_history(timeinfo, (tv_now.tv_usec + 5e4) / 1e5, 0x3f930031, "DBH");
  getLocalTime(&timeinfo);
  gettimeofday(&tv_now, NULL);
  oled_display.add_message_to_history(timeinfo, (tv_now.tv_usec + 5e4) / 1e5, 0x3f930031, "DBI");

  target_time = mktime(&timeinfo) + 10;
  struct tm *target_time_tm = localtime(&target_time);
  Serial.print("Target time: ");
  Serial.printf(asctime(target_time_tm));

  // Setup final display layout
  oled_display.update_wifi(WiFi.status() == WL_CONNECTED, WiFi.localIP()[3]);
  oled_display.update_party_mode(false, *target_time_tm);
  oled_display.update_bell_mute(false, tm{});
  oled_display.redraw();
}

void loop()
{
  delay(500);
  oled_display.update_wifi(WiFi.status() == WL_CONNECTED, WiFi.localIP()[3]);

  // Handle future time callbacks
  tm timeinfo;
  getLocalTime(&timeinfo);
  if (target_time != 0 && mktime(&timeinfo) >= target_time)
  {
    oled_display.update_party_mode(false, tm{});
    target_time = 0;
  }
}