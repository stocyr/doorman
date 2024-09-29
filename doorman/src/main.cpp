#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  // 0x78 = 0x3C << 1 (left shift by 1 bit due to arduino using 7bit I2C addresses)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.println(F("Starting up..."));
  Serial.begin(115200);

  // Initialize I2C
  Wire.begin();

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

  // Set text color
  display.setTextColor(SSD1306_WHITE);

  // Display some text
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Hello,");
  display.println("World!");

  // Show the display buffer on the screen
  display.display();
}

void loop() {
  // Your main code here
}