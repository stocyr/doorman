#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

#include <display.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  // 0x78 = 0x3C << 1 (left shift by 1 bit due to arduino using 7bit I2C addresses)

int sda_pin = 9;
int scl_pin = 10;

DisplayHelper displayHelper;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting up: Connecting to WIFI"));

  // Connect to WiFi
  WiFi.begin("Cyrah", "MornamNueni?");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Get the IP address
  IPAddress ip = WiFi.localIP();
  Serial.print(" Full IP Address: ");
  Serial.println(ip);

  // Initialize I2C

  displayHelper.init();
  displayHelper.oled_draw_wifi(WiFi.status() == WL_CONNECTED, ip[3]);
}

void loop() {
  // Add any additional logic here if needed
  delay(500);
  displayHelper.oled_draw_wifi(WiFi.status() == WL_CONNECTED, WiFi.localIP()[3]);
}