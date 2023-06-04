
#include <PubSubClient.h>

const char* ssid = "wojtek24";
const char* password = "staniszewski123456";
const char* mqtt_server = "192.168.0.10";
const char* mqtt_username = "wojtek"; 
const char* mqtt_password = "69AGjfsuWm8aMviMtTfdMPCpjz68mj";
const int circulation_pin = D4;
const int temp_pin = A0;
const int dolewka_pin = D0;
const int digital_read_pin = D7;
unsigned long czas_dolewki = 0;           // to jest C więc trzaba zadeklarować zmienne globalne na początku

WiFiClient esp_balkon;
PubSubClient client(esp_balkon);

void setup() {
  pinMode(circulation_pin, OUTPUT);
  pinMode(dolewka_pin, OUTPUT);
  pinMode(digital_read_pin, INPUT);
  digitalWrite(circulation_pin, HIGH);       //odwrócone sterowanie być może 
  digitalWrite(dolewka_pin, HIGH);           //odwrócone sterowanie być może 
  

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
      digitalWrite(circulation_pin, LOW);
    } else if (msg == "off") {
      digitalWrite(circulation_pin, HIGH);
    }
  }


  if (strcmp(topic, "balkon/dolewka") == 0) {
    if (msg == "on") {
      czas_dolewki = millis();
      //if dolewka_pin is HIGH for longer than 1200000
      if (millis() - czas_dolewki > 1200000)
       {            // 20 minut
        digitalWrite(dolewka_pin, HIGH);   // odwrócone sterowanie 
      }
      digitalWrite(dolewka_pin, LOW);     // odwrócone sterowanie
    

    } else if (msg == "off") {
      digitalWrite(dolewka_pin, HIGH);    // odwrócone sterowanie
    }
  }


  if (strcmp(topic, "balkon/zapas") == 0) {
    if (msg == "on") {
      digitalWrite(dolewka_pin, LOW);
    } else if (msg == "off") {
      digitalWrite(dolewka_pin, HIGH);
    }
  }


}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("esp_balkon", mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("balkon/cyrkulacja");
      client.subscribe("balkon/dolewka");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
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
  char temp2[10];
  dtostrf(temperature, 5, 2, temp_str);

  client.publish("balkon/temp_zaw", temp_str);

  strcpy(temp2, temp_str);                       // kopiujuę stringa do temp2 - testowo zeby zrobić drugie ogłoszenie
  client.publish("balkon/temp_zaw", temp2);



// temperatura publikownaie do mqtt
  int digital_read_value = digitalRead(digital_read_pin);
  char digital_read_str[10];
  itoa(digital_read_value, digital_read_str, 10);

  client.publish("balkon/digital_read", digital_read_str);
  delay(1000);
}
