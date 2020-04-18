#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include "Led.h"
#include "Scheduler.h"

#define DHTPIN        2 // номер пина, к которому подсоединен датчик
#define LED_DATA_PIN  10 // пин на DIN дисплея
#define LED_CLK_PIN   12 // пин на CLOCK дисплея
#define LED_CS_PIN    11 // пин на CS дисплея

DHT dht(DHTPIN, DHT11);
Adafruit_BMP085 bmp;
Led led(LED_DATA_PIN, LED_CLK_PIN, LED_CS_PIN);
Scheduler scheduler;

static float h = 0, t = 0, bmpT = 0;
static int32_t p = 0;
static char buf[20];
static char buf_float[8];

void displayTask(void* arg)
{
  static bool showPressure = false;
  if (showPressure) {
    dtostrf(p * 0.00750062, 1, 1, buf_float);
    sprintf(buf, "Prs %s", buf_float);
  } else {
    // Проверка удачно прошло ли считывание.
    if (isnan(h) || isnan(t)) {
      //led.print("Error");
      return;
    }
    /* 1 is mininum width, 1 is precision; float value is copied onto str_temp*/
    dtostrf(bmpT, 1, 1, buf_float);
    sprintf(buf, "%s@ %uH", buf_float, round(h));
  }
  led.print(buf);
  showPressure = !showPressure;
}

void readSensorsTask(void* arg)
{
  //Считываем влажность
  h = dht.readHumidity();

  // Считываем температуру
  t = dht.readTemperature();
  bmpT = bmp.readTemperature();

  // Считаваем атм. давление
  p = bmp.readPressure();// Pascals
}

void blinkTask(void* arg)
{
  static bool blinkFlag = false;
  digitalWrite(LED_BUILTIN, blinkFlag ? HIGH : LOW);
  blinkFlag = !blinkFlag;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  led.setup();
  led.print("Start");
  dht.begin();
  if (!bmp.begin()) {
    led.print("Error 085");
    while (1) {}
  }
  scheduler.addRepeatableTask(readSensorsTask, nullptr, 5000, true);
  scheduler.addRepeatableTask(displayTask, nullptr, 3000, true);
  scheduler.addRepeatableTask(blinkTask, nullptr, 100, false);
}

void loop() {
  scheduler.tick();
}
