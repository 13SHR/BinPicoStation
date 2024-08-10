#include "RTClib.h"
#include <TM1637.h>
#include "DHT.h"
#include "lcdgfx.h"

RTC_DS1307 rtc;
TM1637 TM;
DHT dht(14, DHT22);
DisplaySSD1306_128x64_I2C oled(-1);

const int buttonPin = 21;
int oldValue = LOW; // default/idle value for pin 8 is low.

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

  // Input
  pinMode(buttonPin, INPUT);
}

void loop() {
  int newValue = digitalRead(buttonPin);

  DateTime now = rtc.now();
  int hour = now.hour();
  int minutes = now.minute();

  TM.displayTime(hour, minutes, colon); // Display time
  colon = colon ? 0 : 1;

  if (newValue != oldValue) {
    if (newValue == HIGH) {
      float t = dht.readTemperature();
      float h = dht.readHumidity();
      
      oled.clear();
      oled.printFixed(24,  8, "Station Sensors", STYLE_NORMAL);
      sprintf(strBuf, "Temp: %.2f C", t);
      oled.printFixed(0,  24, strBuf, STYLE_NORMAL);
      sprintf(strBuf, "Hum: %.2f %%", h);
      oled.printFixed(0,  32, strBuf, STYLE_NORMAL);
    }
    else {
      oled.clear();
    }
  }
  
  delay(1000);
}
