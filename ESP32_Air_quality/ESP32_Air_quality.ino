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

#include <Wire.h>
#include <SFE_MicroOLED.h>
#include <PMS.h>
#include <DHT.h>
#include <HardwareSerial.h>

HardwareSerial SerialPort(1); // use UART1
PMS pms(SerialPort);
PMS::DATA data;

#define DHTPIN 23
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

#define PIN_RESET -1
#define DC_JUMPER 0
MicroOLED oled(PIN_RESET, DC_JUMPER);

unsigned long previousMillis = 0;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "G6PD_2.4G";
char pass[] = "570610193";

BLYNK_CONNECTED()
{
  Blynk.syncAll();
}

BLYNK_WRITE(V3)
{
  if (param.asInt() == 1)
  {
    digitalWrite(16, LOW );
  }
  else
  {
    digitalWrite(16, HIGH);
  }
}

BLYNK_WRITE(V4)
{
  if (param.asInt() == 1)
  {
    digitalWrite(17, LOW );
  }
  else
  {
    digitalWrite(17, HIGH);
  }
}
BLYNK_WRITE(V5)
{
  if (param.asInt() == 1)
  {
    digitalWrite(18, LOW );
  }
  else
  {
    digitalWrite(18, HIGH);
  }
}
BLYNK_WRITE(V6)
{
  if (param.asInt() == 1)
  {
    digitalWrite(19, LOW );
  }
  else
  {
    digitalWrite(19, HIGH);
  }
}
BLYNK_WRITE(V7)
{
  if (param.asInt() == 1)
  {
    digitalWrite(21, LOW );
  }
  else
  {
    digitalWrite(21, HIGH);
  }
}
BLYNK_WRITE(V8)
{
  if (param.asInt() == 1)
  {
    digitalWrite(22, LOW );
  }
  else
  {
    digitalWrite(22, HIGH);
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  SerialPort.begin(9600, SERIAL_8N1, 25, 27);

  dht.begin();
  Wire.begin();

  oled.begin();
  oled.clear(PAGE);
  oled.clear(ALL);

  digitalWrite(16, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);
  digitalWrite(21, HIGH);
  digitalWrite(22, HIGH);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 2000) {
    previousMillis = currentMillis;

    float humid = dht.readHumidity();
    float temp = dht.readTemperature();

    Serial.print(F("Humidity: "));
    Serial.print(humid);
    Serial.print(F("%  Temperature: "));
    Serial.print(temp);
    Serial.println(F("°C "));

    Blynk.virtualWrite(V1, temp);
    Blynk.virtualWrite(V2, humid);
    
  }

  //------get data from PMS7003------
  if (pms.read(data))
  {
    Blynk.virtualWrite(V0, data.PM_AE_UG_2_5);
    Serial.print(F("PM2.5: "));
    Serial.print(data.PM_AE_UG_2_5);
    Serial.println(F("ug/m3 "));
  }


}
