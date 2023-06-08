/*
 * Ultrasonic Simple
 * Prints the distance read by an ultrasonic sensor in
 * centimeters. They are supported to four pins ultrasound
 * sensors (liek HC-SC04) and three pins (like PING)))
 * and Seeed Studio sensors).
 *
 * The circuit:
 * * Module HR-SC04 (four pins) or PING))) (and other with
 *   three pins), attached to digital pins as follows:
 * ---------------------    --------------------
 * | HC-SC04 | Arduino |    | 3 pins | Arduino |
 * ---------------------    --------------------
 * |   Vcc   |   5V    |    |   Vcc  |   5V    |
 * |   Trig  |   12    | OR |   SIG  |   13    |
 * |   Echo  |   13    |    |   Gnd  |   GND   |
 * |   Gnd   |   GND   |    --------------------
 * ---------------------
 * Note: You do not obligatorily need to use the pins defined above
 * 
 * By default, the distance returned by the read()
 * method is in centimeters. To get the distance in inches,
 * pass INC as a parameter.
 * Example: ultrasonic.read(INC)
 *
 * created 3 Apr 2014
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 * modified 23 Jan 2017
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 * modified 03 Mar 2017
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 * modified 11 Jun 2018
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 *
 * This example code is released into the MIT License.
 */



/*****
 
 All the resources for this project:   fff
 https://randomnerdtutorials.com/
 
*****/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ultrasonic.h>

// for temp reading fron analog grove sensor
#include <math.h>




// Fragment dla DHT sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN D5     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

// end of DHC22 part


//ultrasonic
/*
 * Pass as a parameter the trigger and echo pin, respectively,
 * or only the signal pin (for sensors 3 pins), like:
 * Ultrasonic ultrasonic(13);
 */
Ultrasonic ultrasonic(D6);
int distance;
//ultrasonic end



 
const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0
 
#if defined(ARDUINO_ARCH_AVR)
#define debug  Serial
#elif defined(ARDUINO_ARCH_SAMD) ||  defined(ARDUINO_ARCH_SAM)
#define debug  SerialUSB
#else
#define debug  Serial
#endif

// for temp reading end

// Change the credentials below, so your ESP8266 connects to your led_diode
const char* ssid = "wojtek24";
const char* password = "staniszewski123456";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
//const char* mqtt_server = "192.168.0.10";
const char* mqtt_server = "194.183.54.87";
const char* mqttUser = "wojtek";
const char* mqttPassword = "69AGjfsuWm8aMviMtTfdMPCpjz68mj";


// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient esp_szklarnia;
PubSubClient client(esp_szklarnia);



// cyrkulacja1 - LED - GPIO 4 = D2 on ESP-12E NodeMCU board       // D4 powoduje problem przy restarcie zasilania
const int cyrkulacja1 = 2;
const int zawor = D3;
const int lamp2 = D6;
const int cyrkulacja = D7;
const char* tempTopic = "szklarnia/temp";
const char* humTopic = "szklarnia/hum";
const char* distance = "szklarnia/distance";

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;


// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic szklarnia/podlewanie, you check if the message is either on or off. Turns the lamp GPIO according to the message
  if(topic=="szklarnia/cyrkulacja1"){
      Serial.print("cyrkulacja1 jest ");
      if(messageTemp == "on"){
        digitalWrite(cyrkulacja1, LOW);
     
          Serial.print("zawór On");
     //     delay (50000);
     //    digitalWrite(cyrkulacja1, LOW);
    //    digitalWrite(zawor, LOW);
      
      }
      else if(messageTemp == "off"){
               digitalWrite(cyrkulacja1, HIGH);
        Serial.print("cyrkulacja1 jest  Off");
      }
  }
  Serial.println();


 
 // If a message is received on the topic szklarnia/lamp2, you check if the message is either on or off. Turns the lamp GPIO according to the message
  if(topic=="szklarnia/lamp2"){
      Serial.print("lamp2 jest ");
      if(messageTemp == "on"){
        digitalWrite(lamp2, HIGH);
     
          Serial.print("lamp2 On");

      
      }
      else if(messageTemp == "off"){
               digitalWrite(lamp2, LOW);
        Serial.print("lamp2 Off");
      }
  }
  Serial.println();

   // If a message is received on the topic szklarnia/lamp2, you check if the message is either on or off. Turns the lamp GPIO according to the message
  if(topic=="szklarnia/cyrkulacja"){
      Serial.print("cyrkulacja jest ");
      if(messageTemp == "on"){
        digitalWrite(cyrkulacja, HIGH);
     
          Serial.print("cyrkulacja On");

      
      }
      else if(messageTemp == "off"){
               digitalWrite(cyrkulacja, LOW);
        Serial.print("cyrkulacja Off");
      }
  }
  Serial.println();

}



// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
    if (client.connect("esp_szklarnia")){
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("szklarnia/podlewanie");
        client.subscribe("szklarnia/temp1");
        client.subscribe("szklarnia/cyrkulacja1");
        client.subscribe("szklarnia/lamp2");
        client.subscribe("szklarnia/cyrkulacja");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  pinMode(cyrkulacja1, OUTPUT);
  pinMode(zawor, OUTPUT);
  pinMode(lamp2, OUTPUT);
  pinMode(cyrkulacja, OUTPUT);
  
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

    while (!client.connected()) {
    if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe("szklarnia/cyrkulacja1"); // Subscribe to a specific MQTT topic
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
// DHT22 part do setup
dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

// ebd of dht22 part setup

}

// For this project, you don't need to change anything in the loop function. Basically it ensures that you ESP is connected to your broker
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("esp_test");

  now = millis();



int a = analogRead(pinTempSensor);
 
    float R = 1023.0/a-1.0;
    R = R0*R;
 
     float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
 
  //  Serial.print("temperature = ");
  //  Serial.println(temperature);


    // Publishes new temperature and humidity every 30 seconds
 
 
 if (now - lastMeasure > 10000) {
    
    lastMeasure = now;
    // Publishes Temperature and Humidity values

// konieczna obrobka sygnalu https://www.hobbytronics.co.uk/arduino-float-vars   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  static char temperature1[7];
  dtostrf(temperature, 6, 2, temperature1);    // 6 is mininum width, 2 is precision; float value is copied onto buff
  client.publish("szklarnia/temp1", temperature1);   // 
  //convert now to string now_string
  static char now_string[7];
  dtostrf(now, 6, 0, now_string);

     client.publish("szklarnia/milis", now_string);   //

    }

    // zabezpiecznie termiczne zaworu
 if (temperature>44) { 
  digitalWrite(zawor, LOW);
  }
  
    delay(100);
//DHT part dla void loop

 // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));


    

  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  
  }
//end of dht part void loop

// Get temperature event and print its value.
//sensors_event_t event; // to robilo bład

dht.temperature().getEvent(&event);
if (isnan(event.temperature)) {
  Serial.println(F("Error reading temperature!"));
} else {
  Serial.print(F("Temperature: "));
  Serial.print(event.temperature);
  Serial.println(F("°C"));
  // Publish temperature value
  char temperatureStr[7];
  snprintf(temperatureStr, sizeof(temperatureStr), "%.2f", event.temperature);
  client.publish(tempTopic, temperatureStr);
}

// Get humidity event and print its value.
dht.humidity().getEvent(&event);
if (isnan(event.relative_humidity)) {
  Serial.println(F("Error reading humidity!"));
} else {
  Serial.print(F("Humidity: "));
  Serial.print(event.relative_humidity);
  Serial.println(F("%"));
  // Publish humidity value
  char humidityStr[7];
  snprintf(humidityStr, sizeof(humidityStr), "%.2f", event.relative_humidity);
  client.publish(humTopic, humidityStr);
}
 // Pass INC as a parameter to get the distance in inches
  distance = ultrasonic.read();
  
  Serial.print("Distance in CM: ");
  Serial.println(distance);


} 