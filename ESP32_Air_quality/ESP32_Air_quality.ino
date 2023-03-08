/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  NOTE: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPLHZXTQTqu"
#define BLYNK_TEMPLATE_NAME "Air quality"
#define BLYNK_AUTH_TOKEN "f4FQUVZN0zuZ7F49CbB8KJt7yTreeDUf"


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ssid";
char pass[] = "pass";

BLYNK_CONNECTED()
{
  Blynk.syncAll();
}

BLYNK_WRITE(V3)
{
  if (param.asInt() == 1)
  {
    digitalWrite(16, HIGH);
  }
  else
  {
    digitalWrite(16, LOW);
  }
}

BLYNK_WRITE(V4)
{
  if (param.asInt() == 1)
  {
    digitalWrite(17, HIGH);
  }
  else
  {
    digitalWrite(17, LOW);
  }
}
BLYNK_WRITE(V5)
{
  if (param.asInt() == 1)
  {
    digitalWrite(18, HIGH);
  }
  else
  {
    digitalWrite(18, LOW);
  }
}
BLYNK_WRITE(V6)
{
  if (param.asInt() == 1)
  {
    digitalWrite(19, HIGH);
  }
  else
  {
    digitalWrite(19, LOW);
  }
}
BLYNK_WRITE(V7)
{
  if (param.asInt() == 1)
  {
    digitalWrite(21, HIGH);
  }
  else
  {
    digitalWrite(21, LOW);
  }
}
BLYNK_WRITE(V8)
{
  if (param.asInt() == 1)
  {
    digitalWrite(22, HIGH);
  }
  else
  {
    digitalWrite(22, LOW);
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  //  digitalWrite(16, LOW);
  //  pinMode(16, OUTPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{

  Blynk.virtualWrite(V0, 54);
  Blynk.virtualWrite(V1, 26.2);
  Blynk.virtualWrite(V2, 57.9);
  Blynk.run();
  delay(1000);
}
