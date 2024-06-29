
#include <FastLED.h>

#define NUM_LEDS 1
#define DATA_PIN 8 // D6 on ESP8266
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  Serial.begin(115200); // Start the Serial communication
  Serial.println("Enter a 6-digit HEX code to change the LED color");
}

void loop() {
  if (Serial.available() > 0) {
    String color = Serial.readStringUntil('\n'); // Read the incoming data as string
    color.trim(); // Remove any leading/trailing white spaces or new lines
    
    // Check if received string is a valid 6-digit HEX
    if(color.length() == 6) {
      long number = strtol(&color[0], NULL, 16);
      int r = number >> 16;
      int g = number >> 8 & 0xFF;
      int b = number & 0xFF;

      // Set the LED color
      leds[0] = CRGB(r, g, b);
      FastLED.show();
      Serial.println("Color updated");
    } else {
      Serial.println("Invalid color code. Enter a 6-digit HEX code.");
    }
  }
}
