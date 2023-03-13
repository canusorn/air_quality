/*************************************************************
  สำหรับบอร์ด ESP32

  การต่อสาย
  PMS7003
  TX  -> 25
  RX  -> 27
  GND -> GND
  VCC -> VCC

  DHT -> 26

   ไลบรารี่เพิ่มเติม
   1.PMS โหลดจาก library manager
   2.DHT โหลดจาก library manager
   3.blynk โหลดจาก library manager

 *************************************************************/

/* ดีบัก Blynk ผ่าน Serial */
#define BLYNK_PRINT Serial

/* ข้อมูลจาก Blynk */
#define BLYNK_TEMPLATE_ID "TMPLHZXTQTqu"
#define BLYNK_TEMPLATE_NAME "Air quality"
#define BLYNK_AUTH_TOKEN "f4FQUVZN0zuZ7F49CbB8KJt7yTreeDUf"

#include <WiFi.h>             // ไวไฟ
#include <WiFiClient.h>       // ไวไฟ
#include <BlynkSimpleEsp32.h> // Blynk
#include <PMS.h>              // PMS7003 sensor
#include <DHT.h>              // DHT
#include <HardwareSerial.h>   // Uart กับ PMS7003

// ชื่อไวไฟกับรหัสผ่าน
char ssid[] = "SSID";
char pass[] = "PASS";

HardwareSerial SerialPort(1); // ใช้งาน UART1
PMS pms(SerialPort);          // สร้าง object ชื่อ pms สำหรับเซนเซอร์ฝุ่น
PMS::DATA data;               // สร้างตัวแปร data เก็บค่าเซนเซอร์ฝุ่น

#define DHTPIN 26     // ตั้งค่าขาต่อ DHT
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE); // สร้าง object ชื่อ dht

unsigned long previousMillis = 0; // ตัวแปรเก็บค่าการ delay

BLYNK_CONNECTED() // เรียกเมื่อ blynk เชื่อมต่อสำเร็จ
{
  Blynk.syncAll();
}

BLYNK_WRITE(V3) // เรียกเมือกด Virtual Pin 3 (OFF-ON)
{
  if (param.asInt() == 1) // ถ้าค่าจาก Blynk เป็น1 (มีการกดปุ่ม)
  {
    digitalWrite(16, LOW);     // output เป็น low (relay active low)
    delay(500);                // รอ 0.5 วินาที
    digitalWrite(16, HIGH);    // output เป็น high
    Blynk.virtualWrite(V3, 0); // อัพเดทค่า Virtual pin
  }
}
BLYNK_WRITE(V4) // เรียกเมือกด Virtual Pin 4 (Timer)
{
  if (param.asInt() == 1) // ถ้าค่าจาก Blynk เป็น1 (มีการกดปุ่ม)
  {
    digitalWrite(17, LOW);     // output เป็น low (relay active low)
    delay(500);                // รอ 0.5 วินาที
    digitalWrite(17, HIGH);    //  เป็น high
    Blynk.virtualWrite(V4, 0); // อัพเดทค่า Virtual pin
  }
}
BLYNK_WRITE(V5) // เรียกเมือกด Virtual Pin 5 (Mode)
{
  if (param.asInt() == 1) // ถ้าค่าจาก Blynk เป็น1 (มีการกดปุ่ม)
  {
    digitalWrite(32, LOW);     // output เป็น low (relay active low)
    delay(500);                // รอ 0.5 วินาที
    digitalWrite(32, HIGH);    //  เป็น high
    Blynk.virtualWrite(V5, 0); // อัพเดทค่า Virtual pin
  }
}
BLYNK_WRITE(V6) // เรียกเมือกด Virtual Pin 6 (Max Speed)
{
  if (param.asInt() == 1) // ถ้าค่าจาก Blynk เป็น1 (มีการกดปุ่ม)
  {
    digitalWrite(33, LOW);     // output เป็น low (relay active low)
    delay(500);                // รอ 0.5 วินาที
    digitalWrite(33, HIGH);    //  เป็น high
    Blynk.virtualWrite(V6, 0); // อัพเดทค่า Virtual pin
  }
}
BLYNK_WRITE(V7) // เรียกเมือกด Virtual Pin 7 (Speed)
{
  if (param.asInt() == 1) // ถ้าค่าจาก Blynk เป็น1 (มีการกดปุ่ม)
  {
    digitalWrite(18, LOW);     // output เป็น low (relay active low)
    delay(500);                // รอ 0.5 วินาที
    digitalWrite(18, HIGH);    //  เป็น high
    Blynk.virtualWrite(V7, 0); // อัพเดทค่า Virtual pin
  }
}
BLYNK_WRITE(V8) // เรียกเมือกด Virtual Pin 8 (Anion)
{
  if (param.asInt() == 1) // ถ้าค่าจาก Blynk เป็น1 (มีการกดปุ่ม)
  {
    digitalWrite(19, LOW);     // output เป็น low (relay active low)
    delay(500);                // รอ 0.5 วินาที
    digitalWrite(19, HIGH);    //  เป็น high
    Blynk.virtualWrite(V8, 0); // อัพเดทค่า Virtual pin
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);                       // Serail ทำงาน
  SerialPort.begin(9600, SERIAL_8N1, 25, 27); // Hardware Serial Port 1 ทำงาน ใช้สือสารกับ PMS7003

  dht.begin(); // dht ทำงาน

  // เรื่องต้นให้ปิด relay (Active low)
  digitalWrite(16, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);
  digitalWrite(32, HIGH);
  digitalWrite(33, HIGH);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);

  // เชือมต่อกับ Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run(); // blynk

  unsigned long currentMillis = millis();     // เก็บค่าเวลาปัจจุบัน
  if (currentMillis - previousMillis >= 2500) // เทียบเวลาปัจจุบันกับการทำงานครั้งก่อน ให้ทำงานทุก 2500 ms
  {
    previousMillis = currentMillis; // อัพเดทเวลาการทำงานล่าสุด

    float humid = dht.readHumidity();   // เรียกค่าความชื้น
    float temp = dht.readTemperature(); // เรียกค่าอุณหภูมิ

    // แสดงค่าทาง Serial
    Serial.print(F("Humidity: "));
    Serial.print(humid);
    Serial.print(F("%  Temperature: "));
    Serial.print(temp);
    Serial.println(F("°C "));

    // อัพเดทค่าขึ้น Blynk
    Blynk.virtualWrite(V1, temp);
    Blynk.virtualWrite(V2, humid);
  }

  //------เมื่อเซนเซอร์ส่งค่ามา------
  if (pms.read(data))
  {
    Blynk.virtualWrite(V0, data.PM_AE_UG_2_5); // อัพเดทค่า pm2.5 ไปยัง virtual pin 0

    // แสดงค่าทาง Serial
    Serial.print(F("PM2.5: "));
    Serial.print(data.PM_AE_UG_2_5);
    Serial.println(F("ug/m3 "));
  }
}
