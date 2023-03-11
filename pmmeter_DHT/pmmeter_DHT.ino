/*
   -PMS7003-
   5V - VCC
   GND - GND
   D4  - TX
   D3  - RX(not use in this code)

*/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPLHZXTQTqu"
#define BLYNK_TEMPLATE_NAME "Air quality"
#define BLYNK_AUTH_TOKEN "f4FQUVZN0zuZ7F49CbB8KJt7yTreeDUf"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <SFE_MicroOLED.h>
#include <PMS.h>
#include <DHT.h>
#include <SoftwareSerial.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SSID";
char pass[] = "PASS";

SoftwareSerial pmsSerial(D4, D3); // RX,TX
PMS pms(pmsSerial);
PMS::DATA data;

#define DHTPIN D7
// Uncomment whatever type you're using!
// #define DHTTYPE DHT11 // DHT 11
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
uint8_t dht_time;

float humid, temp;

#define PIN_RESET -1
#define DC_JUMPER 0
MicroOLED oled(PIN_RESET, DC_JUMPER);

unsigned long previousMillis = 0;

void setup()
{
  Serial.begin(115200);
  pmsSerial.begin(9600);
  dht.begin();
  Wire.begin();

  pinMode(D6, OUTPUT);
  pinMode(D8, OUTPUT);
  digitalWrite(D6, HIGH);
  digitalWrite(D8, LOW);

  //------Display LOGO at start------
  oled.begin();
  oled.clear(PAGE);
  oled.clear(ALL);
  oled.setFontType(0);
  oled.setCursor(0, 0);
  oled.print("PMMeter");
  oled.display();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{

  //------get data from PMS7003------
  if (pms.read(data))
  {
    Blynk.virtualWrite(V9, data.PM_AE_UG_2_5);
    Serial.print(F("PM2.5: "));
    Serial.print(data.PM_AE_UG_2_5);
    Serial.println(F("ug/m3 "));
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 2500)
  { // run every 1 second
    previousMillis = currentMillis;

    humid = dht.readHumidity();
    temp = dht.readTemperature();

    Serial.print(F("Humidity: "));
    Serial.print(humid);
    Serial.print(F("%  Temperature: "));
    Serial.print(temp);
    Serial.println(F("°C "));

    display_update();
  }


}

void display_update()
{

  oled.clear(PAGE);
  oled.setFontType(0);
  oled.setCursor(0, 0);
  oled.println("-Sensors-");
  oled.setCursor(0, 15);
  oled.print("PM2.5: ");
  oled.print(data.PM_AE_UG_2_5, 0);
  oled.setCursor(0, 27);
  oled.print("T: ");
  oled.print(temp, 1);
  oled.setCursor(0, 39);
  oled.print("H: ");
  oled.print(humid, 1);

  oled.display();

}
