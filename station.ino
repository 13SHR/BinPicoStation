#include <dht.h>
#include "RTClib.h"
#include <TM1637.h>

RTC_DS1307 rtc;
TM1637 TM;
dht DHT;

int colon = 0; // boolean to make the colon blink each second

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  Serial1.println("Station starting up!");

  if (! rtc.begin()) {
    Serial1.println("RTC not found. Exiting");
    Serial1.flush();
    abort();
  }

  TM.begin(2, 3, 4);       //  clockpin, datapin, #digits
  TM.displayClear();
  TM.setBrightness(7);     // full brightness, default is 3

  pinMode(14, INPUT);
}

void loop() {
  DateTime now = rtc.now();
  int hour = now.hour();
  int minutes = now.minute();

  TM.displayTime(hour, minutes, colon); // Display time
  colon = colon ? 0 : 1;

  Serial1.print(DHT.temperature);
  Serial1.print(" / ");
  Serial1.print(DHT.humidity);
  Serial1.println();

  delay(1000); // this speeds up the simulation
}
