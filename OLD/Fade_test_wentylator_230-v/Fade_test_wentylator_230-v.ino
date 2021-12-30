/*
  Fade

  This example shows how to fade an LED on pin 9 using the analogWrite()
  function.

  The analogWrite() function uses PWM, so if you want to change the pin you're
  using, be sure to use another PWM capable pin. On most Arduino, the PWM pins
  are identified with a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Fade
*/

int led = D2;           // the PWM pin the LED is attached to
int brightness = 1020;    // how bright the LED is
int fadeAmount = 50;    // how many points to fade the LED by
int sensorPin = A0;
int sensorValue = 0;

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(led, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {

   // read the value from the sensor:
  // sensorValue = analogRead(sensorPin);
  // set the brightness of pin 9:
  analogWrite(led, sensorValue);

  // change the brightness for next time through the loop:
//  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
 // if (brightness <= 0 || brightness >= 255) {
 //   fadeAmount = -fadeAmount;
 // }
  // wait for 30 milliseconds to see the dimming effect
 // delay(10000);
}
