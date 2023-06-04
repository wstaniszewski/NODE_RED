#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <MQTTClient.h>

WiFiClient net;
MQTTClient mqtt;

const char* ssid = "wojtek";
const char* password = "staniszewski123456";
const char* mqttServer = "194.183.54.87";
const int mqttPort = 1883;
const char* mqttUser = "wojtek";
const char* mqttPassword = "69AGjfsuWm8aMviMtTfdMPCpjz68mj";

char buffer[50];

void fun_szklarnia_cyrkulacja1_(String& topic, String& payload) {
  Serial.println(payload);
  if (payload == "on") {
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(2, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  mqtt.begin(mqttServer, mqttPort, net);
  mqtt.onMessage(fun_szklarnia_cyrkulacja1_);
  
  while (!mqtt.connect("NodeMCU", mqttUser, mqttPassword)) {
    Serial.println("Connecting to MQTT...");
    delay(1000);
  }

  mqtt.subscribe("szklarnia/cyrkulacja1");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
  }

  if (!mqtt.connected()) {
    while (!mqtt.connect("NodeMCU", mqttUser, mqttPassword)) {
      Serial.println("Connecting to MQTT...");
      delay(1000);
    }

    mqtt.subscribe("szklarnia/cyrkulacja1");
  }

  mqtt.loop();

  // Publish sensor data
  mqtt.publish("szklarnia/volty_node", String(analogRead(A0)).c_str());
  mqtt.publish("szklarnia/batt_perc", String(analogRead(A0)).c_str());
  mqtt.publish("szklarnia/temp1", String(analogRead(A0)).c_str());

  delay(1000);
}
