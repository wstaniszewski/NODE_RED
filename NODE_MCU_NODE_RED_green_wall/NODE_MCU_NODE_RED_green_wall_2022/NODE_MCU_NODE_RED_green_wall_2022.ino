


/*****
 
 All the resources for this project:
 https://randomnerdtutorials.com/
 
*****/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// for temp reading fron analog grove sensor
#include <math.h>
 
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
const char* mqtt_server = "192.168.0.2";
//const int mqttPort = 12948;
const char* mqttUser = "wojtek";
const char* mqttPassword = "69AGjfsuWm8aMviMtTfdMPCpjz68mj";


// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient esp_test2;
PubSubClient client(esp_test2);



// led1 - LED - GPIO 4 = D2 on ESP-12E NodeMCU board
const int led1 = D3;
const int zawor = D4;
const int lamp3 = D5;



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

  // If a message is received on the topic kuchnia/podlewanie, you check if the message is either on or off. Turns the lamp GPIO according to the message
  if(topic=="kuchnia/podlewanie"){
      Serial.print("esp_test2  jest ");
      if(messageTemp == "on"){
        digitalWrite(led1, HIGH);
        digitalWrite(zawor, HIGH);
        digitalWrite(lamp3, HIGH);
          Serial.print("On");
     //     delay (50000);
     //    digitalWrite(led1, LOW);
    //    digitalWrite(zawor, LOW);
      
      }
      else if(messageTemp == "off"){
        digitalWrite(led1, LOW);
               digitalWrite(zawor, LOW);
                      digitalWrite(lamp3, LOW);
        Serial.print("Off");
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
    if (client.connect("esp_test2", mqttUser, mqttPassword)) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("kuchnia/podlewanie");
        client.subscribe("kuchnia/temp_zaw");
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
  pinMode(led1, OUTPUT);
    pinMode(zawor, OUTPUT);

  pinMode(lamp3, OUTPUT);
  
  
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

// For this project, you don't need to change anything in the loop function. Basically it ensures that you ESP is connected to your broker
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("esp_test2");

  now = millis();



int a = analogRead(pinTempSensor);
 
    float R = 1023.0/a-1.0;
    R = R0*R;
 
     float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
 
    Serial.print("temperature = ");
    Serial.println(temperature);


    // Publishes new temperature and humidity every 30 seconds
 
 
 if (now - lastMeasure > 10000) {
    
    lastMeasure = now;
    // Publishes Temperature and Humidity values

// konieczna obrobka sygnalu https://www.hobbytronics.co.uk/arduino-float-vars   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  static char temperature1[7];
  dtostrf(temperature, 6, 2, temperature1 );
    
  client.publish("kuchnia/temp_zaw", temperature1);   //wywala bad
   
    }

    // zabezpiecznie termiczne zaworu
 if (temperature>44) { 
  digitalWrite(zawor, LOW);
  }
  
    delay(100);


} 
