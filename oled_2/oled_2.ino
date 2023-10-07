#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define OLED display (assuming a resolution of 128x64)
Adafruit_SSD1306 display(128, 32, &Wire);

void setup() {
  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // OLED address 0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();  // Clear the buffer
}

void loop() {
  display.setTextSize(1);  // Text size
  display.setTextColor(SSD1306_WHITE);  // Text color
  display.setCursor(0,0);  // Start position
  display.print(F("test"));  // Print text
  display.display();  // Display buffer
  delay(2000);  // Delay 2 seconds
  display.clearDisplay();  // Clear the buffer
  delay(2000);  // Delay 2 seconds
}
