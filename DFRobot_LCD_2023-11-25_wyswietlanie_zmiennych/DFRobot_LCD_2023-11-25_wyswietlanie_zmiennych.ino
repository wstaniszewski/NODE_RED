
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

// Pin configuration for the TFT display
#define TFT_CS     D5  // Define as per your connection
#define TFT_RST    D4  // Define as per your connection
#define TFT_DC     D2  // Define as per your connection

// Initialize Adafruit ST7735 TFT library
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  // Initialize the display 128x160 pixel
  //tft.initR(INITR_144GREENTAB); // Initialize a ST7735S chip, black tab



 tft.initR(INITR_BLACKTAB);      // Initialize a ST7735S chip, black tab

 //tft.setFont(&FreeSans9pt7b);    // Set the font to FreeSans 9pt
  tft.setTextColor(ST77XX_WHITE); // Set text color
  
// Adjust the cursor position
  int xOffset = 10;  // Adjust this value as needed
  int yOffset = 10;  // Adjust this value as needed
  tft.setCursor(xOffset, yOffset); // Set cursor position with offset

  tft.setRotation(3);             // Rotate the display by 90 degrees
  tft.fillScreen(ST77XX_BLACK);   // Clear the screen to black
  tft.setTextSize(1);             // Set text size
  tft.setTextColor(ST77XX_WHITE); // Set text color
  //tft.setCursor(2, 2);            // Set cursor to top-left corner


  
}

void loop() {
  //set font TomThumb Font
 
  
  tft.fillScreen(ST77XX_BLACK);        // Clear the screen
  tft.setCursor(5, 5);                 // Reset cursor to top-left
  tft.print("Millis: ");               // Print text to the screen
  tft.println(millis());               // Print the value of millis()
  delay(500);                          // Wait for half a second
}
