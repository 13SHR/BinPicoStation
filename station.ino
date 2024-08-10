#include "RTClib.h"
#include <TM1637.h>
#include "DHT.h"
#include "lcdgfx.h"

RTC_DS1307 rtc;
TM1637 TM;
DHT dht(14, DHT22);
DisplaySSD1306_128x64_I2C oled(-1);

const int buttonPin = 27;
int oldValue = LOW;

char strBuf[50];

bool colon = true; // boolean to make the colon blink each second
unsigned long previousClock;

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
  unsigned long currentClock = millis(); // Timer for clock

  DateTime now = rtc.now();
  int hour = now.hour();
  int minutes = now.minute();

  if(currentClock - previousClock >= 1000)
  {
    previousClock += 1000;

    TM.displayTime(hour, minutes, colon);
    colon = !colon;
  }

  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
  // Display the sensors data only if the button is pressed
  if (oldValue != newValue) {
    oldValue = newValue;
    if (newValue == LOW) {
      oled.clear();
      oled.printFixed(24,  8, "Station Sensors", STYLE_NORMAL);
      sprintf(strBuf, "Temp: %.2f C", t);
      oled.printFixed(5,  24, strBuf, STYLE_NORMAL);
      sprintf(strBuf, "Hum: %.2f %%", h);
      oled.printFixed(5,  32, strBuf, STYLE_NORMAL);
    } else {
      oled.clear();
    }
  }
  delay(5);
}
