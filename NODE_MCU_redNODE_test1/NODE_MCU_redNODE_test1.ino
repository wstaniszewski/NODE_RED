#include <ESP8266WiFi.h>

#include <PubSubClient.h>

 char* ssid = "Cisco24"; //ZMIENIC na swoje
const char* password = "staniszewski1234"; //ZMIENIC na swoje 
const char* mqtt_server = "192.168.8.2"; //ZMIENIC na swoje
const char* deviceName = "ESP"; //poki co nie trzeba zmieniac 
//ale przy wiekszej ilosci urzaden kazde musi miec swoja nazwe

const char* startMessageTopic = "esp8266/message"; //temat do wyslania wiadomosci
const char* pinTopic = "esp8266/gpio"; //temat do odbioru wiadomosci
const int ledPin = LED_BUILTIN; //numer pinu diody, ZMIENIC JESLI TRZEBA

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect()
{
  bool ctd = false;
  //funkcja jest wywolywana jesli utracono polaczenie z serwerema
  while(!ctd)
  {
    Serial.print("Laczenie z serwerem...");
    if(client.connect(deviceName))
    {
      ctd = true;
      Serial.println("Polaczono!");
    }
    else
    {
      Serial.print(".");
      delay(1000);
    }
  }
}

void odbiorWiadomosci(String temat, byte* zawartosc, unsigned int dlugosc)
{
  String pomoc;
  Serial.println("Odebrano wiadomosc:");
  Serial.print("\tTemat: ");
  Serial.println(temat);
  Serial.print("\tTresc: \"");
  for(int i=0; i<dlugosc; i++) 
  {
    Serial.print((char)zawartosc[i]);
    pomoc += (char)zawartosc[i];
  }
  Serial.println("\"");

  if(temat == pinTopic)
  {
    if(pomoc == "1")
    {
     
      digitalWrite(ledPin, LOW);
      Serial.println("LED1: ON"); 
    }
    else if(pomoc == "0")
    {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED1: OFF");
    }
    else
      Serial.println("Nieznana komenda, nie wiem co mam z tym zrobic");
  }
}

void ustawienieWifi()
{
  delay(10);
  Serial.println();
  Serial.print("Laczenie z ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Polaczona z wifi.\nESP otrzymalo adres IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin,OUTPUT);
  ustawienieWifi(); //polaczenie z wifi
  delay(1000);
  client.setServer(mqtt_server, 1883); //ustawienie serwera  mqtt
  client.connect(deviceName); //polaczenie z podana nazwa
  client.subscribe(pinTopic); //ustawienie nasluchiwania w podanym temacie
  client.setCallback(odbiorWiadomosci); //ustawienie funkcji do odbioru wiadomosci
  client.publish(startMessageTopic, "Hello from ESP32"); //wyslanie pierwszej wiadomosci
}

void loop() {
  
  if (!client.connected()) //jesli klient zostal rozlaczony
  {
   reconnect(); //polacz ponownie
   client.publish(startMessageTopic, "Hello from ESP32"); //wysliij wiadomoc powitalna
  }
  if(!client.loop())
    client.connect(deviceName); //upewnienie sie, ze klient jest stale podlaczony
}
