#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "wojtek 24";
const char* password = "staniszewski123456";
const char* mqtt_server = "192.168.0.10";
const int circulation_pin = D2;
const int temp_pin = A0;

WiFiClient esp_balkon;
PubSubClient client(esp_balkon);

void setup() {
  pinMode(circulation_pin, OUTPUT);
  digitalWrite(circulation_pin, LOW);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  if (strcmp(topic, "balkon/cyrkulacja") == 0) {
    if (msg == "on") {
      digitalWrite(circulation_pin, HIGH);
    } else if (msg == "off") {
      digitalWrite(circulation_pin, LOW);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("esp_balkon")) {
      Serial.println("connected");
      client.subscribe("balkon/cyrkulacja");
    } else {
      Serial.print("failed, rc=");
      Serial.print(clieznt.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int temp_value = analogRead(temp_pin);
  float temperature = ((temp_value * 3.3) / 1024.0) * 100.0; // Assuming a 10mV/°C linear temperature sensor
  char temp_str[10];
  dtostrf(temperature, 5, 2, temp_str);

  client.publish("balkon/temp_zaw", temp_str);
  
  delay(1000);
}
