#include <Arduino.h>
#include <U8x8lib.h>
#include <Wire.h>

//U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* clock=*/ PIN_WIRE_SCL, /* data=*/ PIN_WIRE_SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display

void setup(void) {
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate
  while(!Serial); // Wait for the serial port to connect. Needed for native USB
  Serial.println("Initializing display...");

  u8x8.begin();
  u8x8.setFlipMode(0);   // set number from 1 to 3, the screen word will rotary 180
  Serial.println("Display initialized.");
}

void loop(void) {
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  u8x8.print("Hello World!www");

  // Example of using Serial.print() for debugging:
  Serial.println("Displayed 'Hello World!www'");
  
  // Simple delay to prevent flooding the serial output.
  delay(1000);
}
