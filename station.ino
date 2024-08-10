#include "RTClib.h"
#include <TM1637.h>
#include "DHT.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h> // Needed for the OLED Adafruit library

RTC_DS1307 rtc;
TM1637 TM;
DHT dht(14, DHT22);
Adafruit_SSD1306 oled(128, 64, &Wire);

char strBuf[50];

int colon = 0; // boolean to make the colon blink each second

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  Serial1.println("Station starting up!");

  // RTC Clock
  if (! rtc.begin()) {
    Serial1.println("RTC not found. Exiting");
    Serial1.flush();
  }

  // 4 digit display
  TM.begin(2, 3, 4);       //  clockpin, datapin, #digits
  TM.displayClear();
  TM.setBrightness(7);     // full brightness, default is 3

  // Humidity and Temperature sensor
  dht.begin();

  // OLED display
  if (oled.begin()) {
    Serial1.println("Tout est bon");
  } else {
    Serial1.println("C'est ici que ça coince");
  }
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
}

void loop() {
  DateTime now = rtc.now();
  int hour = now.hour();
  int minutes = now.minute();

  TM.displayTime(hour, minutes, colon); // Display time
  colon = colon ? 0 : 1;

  float t = dht.readTemperature();
  float h = dht.readHumidity();
  Serial1.print(t);
  Serial1.print(" / ");
  Serial1.print(h);
  Serial1.println();

  oled.setCursor(40, 0);
  oled.println("Station Sensors:");
  
  oled.setCursor(60, 20);
  oled.print("T:");
  oled.print(t);
  oled.println(" °C");

  oled.setCursor(60, 30);
  oled.print("H: ");
  oled.print(h);
  oled.println(" %");

  delay(1000); // this speeds up the simulation
}
