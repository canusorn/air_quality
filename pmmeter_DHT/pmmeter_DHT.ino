/*************************************************************
* สำหรับบอร์ด ESP8266
* 
* การต่อสาย
* -PMS7003-
* 5V - VCC
* GND - GND
* D4  - TX
* D3  - RX(not use in this code)
*
* ไลบรารี่เพิ่มเติม
* 1.PMS โหลดจาก library manager
* 2.DHT โหลดจาก library manager
* 3.blynk โหลดจาก library manager

*************************************************************/

// ดีบัก Blynk ผ่าน Serial
#define BLYNK_PRINT Serial

/* ข้อมูลจาก Blynk */
#define BLYNK_TEMPLATE_ID "TMPLHZXTQTqu"
#define BLYNK_TEMPLATE_NAME "Air quality"
#define BLYNK_AUTH_TOKEN "f4FQUVZN0zuZ7F49CbB8KJt7yTreeDUf"

#include <ESP8266WiFi.h>        // wifi
#include <BlynkSimpleEsp8266.h> // blynk
#include <Wire.h>               // i2c กับจอ oled
#include <SFE_MicroOLED.h>      // จอ oled
#include <PMS.h>                // PMS7003 sensor
#include <DHT.h>                // dht
#include <SoftwareSerial.h>     // Uart กับ PMS7003

// ชื่อไวไฟกับรหัสผ่าน
char ssid[] = "SSID";
char pass[] = "PASS";

SoftwareSerial pmsSerial(D4, D3); // ใช้งาน software serial RX,TX ที่ขา D4,D3
PMS pms(pmsSerial);               // สร้าง object ชื่อ pms สำหรับเซนเซอร์ฝุ่น
PMS::DATA data;                   // สร้างตัวแปร data เก็บค่าเซนเซอร์ฝุ่น

#define DHTPIN D7     // ตั้งค่าขาต่อ DHT
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE); // สร้าง object ชื่อ dht

float humid, temp; // ตัวแปรเก็บค่า humid temperature

// ตั้งค่าการทำงานจอ oled
#define PIN_RESET -1
#define DC_JUMPER 0
MicroOLED oled(PIN_RESET, DC_JUMPER);

unsigned long previousMillis = 0; // ตัวแปรเก็บค่าการ delay

void setup()
{
  Serial.begin(115200);  // Serail ทำงาน
  pmsSerial.begin(9600); // Software Serial ทำงาน ใช้สือสารกับ PMS7003

  dht.begin();  // dht ทำงาน
  Wire.begin(); // i2c ทำงาน เพื่อสื่อสารกับจอ oled

  // จ่ายไฟให้ dht
  pinMode(D6, OUTPUT);
  pinMode(D8, OUTPUT);
  digitalWrite(D6, HIGH);
  digitalWrite(D8, LOW);

  // เรื่มต้นทำงาน จอ oled
  oled.begin();
  oled.clear(PAGE);
  oled.clear(ALL);
  oled.setFontType(0);
  oled.setCursor(0, 0);
  oled.print("PMMeter");
  oled.display();

  // เชือมต่อกับ Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{

  Blynk.run(); // blynk

  //------ เมื่อเซนเซอร์ PMS7003 ส่งค่ามา ------
  if (pms.read(data))
  {
    Blynk.virtualWrite(V9, data.PM_AE_UG_2_5); // ส่งค่า pm2.5 ไป V9

    // แสดงค่าฝุ่นทาง Serial monitor
    Serial.print(F("PM2.5: "));
    Serial.print(data.PM_AE_UG_2_5);
    Serial.println(F("ug/m3 "));
  }

  unsigned long currentMillis = millis();     // เก็บค่าเวลาปัจจุบัน
  if (currentMillis - previousMillis >= 2500) // เทียบเวลาปัจจุบันกับการทำงานครั้งก่อน ให้ทำงานทุก 2500 ms
  {
    previousMillis = currentMillis; // อัพเดทเวลาการทำงานล่าสุด

    humid = dht.readHumidity();   // เรียกค่าความชื้น
    temp = dht.readTemperature(); // เรียกค่าอุณหภูมิ

    // อัพเดทค่าขึ้น Blynk
    Blynk.virtualWrite(V10, humid);
    Blynk.virtualWrite(V11, temp);

    // แสดงค่าทาง Serial
    Serial.print(F("Humidity: "));
    Serial.print(humid);
    Serial.print(F("%  Temperature: "));
    Serial.print(temp);
    Serial.println(F("°C "));

    display_update(); // แสดงค่าขึ้นจอ oled
  }
}

void display_update() // แสดงค่าบนจอ oled
{
 
  oled.clear(PAGE);// เครียร์บนจอ
  oled.setFontType(0);// เลือก font
  oled.setCursor(0, 0);// เลือกตำแหน่ง
  oled.println("-Sensors-");// แสดงตัวอักษร
  oled.setCursor(0, 15);// เลือกตำแหน่ง
  oled.print("PM2.5: ");// แสดงตัวอักษร
  oled.print(data.PM_AE_UG_2_5, 0);// แสดงค่า pm2.5
  oled.setCursor(0, 27);// เลือกตำแหน่ง
  oled.print("T: ");// แสดงตัวอักษร
  oled.print(temp, 1);// แสดงค่าอุณหภูมิ
  oled.setCursor(0, 39);// เลือกตำแหน่ง
  oled.print("H: ");// แสดงตัวอักษร
  oled.print(humid, 1);// แสดงค่าความชื้น

  oled.display(); // อัพเดทค่าบนจอ oled
}
