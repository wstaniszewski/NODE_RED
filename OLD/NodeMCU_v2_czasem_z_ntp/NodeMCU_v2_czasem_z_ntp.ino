/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  You’ll need:
   - Blynk App (download from AppStore or Google Play)
   - NodeMCU board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial      #do druku znaków z treminala w telefonie


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "XN4jBP2y1NTfTb6cll3kFWgCQY_HlP4O";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Dom";
char pass[] = "mariner1234";


const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V1);


// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V1 Slider value is: ");
  Serial.println(pinValue);
}

int net1Pin = D3;
int pinValue =1;
int pinValue_over=0;

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);



// Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Clear the terminal content
  terminal.clear();

  // This will print Blynk Software version to the Terminal Widget when
  // your hardware gets connected to Blynk Server
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.println(F("Type 'Marco' and get a reply, or type"));
  terminal.println(F("anything else and get it printed back."));
  terminal.flush();

pinMode(net1Pin, OUTPUT);
pinMode (pinValue_over, OUTPUT);


// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(0);
}

void loop()
{
  Blynk.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
Blynk.virtualWrite(5, millis() / 1000);

 timeClient.update();

// Serial.print(daysOfTheWeek[timeClient.getDay()]);
 // Serial.print(", ");
  //Serial.print(timeClient.getHours());
  //Serial.print(":");
  //Serial.print(timeClient.getMinutes());
  //Serial.print(":");
  // Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());

  Blynk.virtualWrite(4, (daysOfTheWeek[timeClient.getDay()])
  );
  // terminal.println("test");
 //   terminal.println();

// CZAS  //////////////

  timeClient.update();
  
  int currentHour = timeClient.getHours();
  currentHour=currentHour+1;

  Serial.println(currentHour);  
//wieksza pentla sterowania Blynkiem

pinValue=digitalRead(D4);
pinValue_over=digitalRead(D5);

// if (pinValue == 0 ) { Serial.print ("  pinValue is  0");}
Serial.print ("Pin value :   ");
Serial.print (pinValue);
Serial.print ("      ");
//mała patla czasowa


// brakuje wlacznika (ON) w czasie zakazanym

if  (pinValue==1)
{
   digitalWrite(net1Pin, LOW);      // uruchamia router
          Serial.print ("   Cisco24  wylaczona przez rodzica");
          terminal.println (F("   Cisco24  wylaczona przez rodzica"));
}



else {

      if ((currentHour >  (8)  ) && (currentHour < 20 )) 
      //if ((currentHour >  (8)  ) && (currentHour < 20))

        //||  (pinValue==0)

      {
              digitalWrite(net1Pin, HIGH);
              Serial.print ("   Cisco24  is On bo pozwala na to czas lub rodzic  ");
               terminal.println (F("   Cisco24  is On bo pozwala na to czas   "));
                      }


//        else if (pinValue_over=1)  //ale tak to po czasie jest ciagle override   - jak zachowuje siwe pin??
  //      {
    //          digitalWrite(net1Pin, HIGH);
      //        Serial.print ("   Cisco24  is On OVERRIDE ON  ");
        //       terminal.println (F("   Cisco24  is On OVERRIDE ON     "));
          //            }

       else{
               digitalWrite(net1Pin, LOW);
               Serial.print ("   Cisco24  is off bo poza czasem  ");
              terminal.println (F("   Cisco24  is off bo poza czasem    "));
          

}}
  //Serial.println(timeClient.getFormattedTime());


}
