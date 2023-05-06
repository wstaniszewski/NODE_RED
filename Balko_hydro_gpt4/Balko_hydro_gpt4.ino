#include <DHT.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor (D4 on ESP-12E)
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  ...
  dht.begin(); // Add this line in the setup function
  ...
}

float readTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return 0.0;
  }
  return t;
}

void loop() {
  ...
  // Replace the following lines with your temperature sensor reading logic
  float temperature = readTemperature();
  char tempStr[8];
  dtostrf(temperature, 6, 2, tempStr);
  client.publish(temperatureTopic, tempStr);
  delay(30000); // Publish the temperature every 30 seconds
}
