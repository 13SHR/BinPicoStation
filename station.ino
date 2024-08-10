#include "RTClib.h"
#include <TM1637.h>
#include "DHT.h"
#include "lcdgfx.h"

RTC_DS1307 rtc;
TM1637 TM;
DHT dht(14, DHT22);
DisplaySSD1306_128x64_I2C oled(-1);

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
  oled.setFixedFont( ssd1306xled_font6x8 );
  oled.begin();
  oled.clear();
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

  oled.printFixed(0,  8, "Station Sensors", STYLE_NORMAL);
  oled.printFixed(0,  16, "Placeholder Temperature", STYLE_NORMAL);
  oled.printFixed(0,  24, "Placeholder Humidity", STYLE_NORMAL);
  
  delay(1000);
}
