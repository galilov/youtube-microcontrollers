#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>

#define DHTPIN 2 // номер пина, к которому подсоединен датчик

// Инициируем датчик

DHT dht(DHTPIN, DHT11);
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  dht.begin();
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
}

void loop() {
  // Задержка 2 секунды между измерениями
  delay(2000);
  //Считываем влажность
  float h = dht.readHumidity();

  // Считываем температуру
  float t = dht.readTemperature();

  // Проверка удачно прошло ли считывание.
  if (isnan(h) || isnan(t)) {

    Serial.println("Не удается считать показания");
    return;
  }

  float bmpT = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude();

  Serial.print("Влажность: ");
  Serial.print(h);
  Serial.print(" %\tТемпература: ");
  Serial.print(t);
  Serial.print("/");
  Serial.print(bmpT);
  Serial.print(" *C\tДавление:");
  Serial.print(pressure);
  Serial.print("Па\tВысота:");
  Serial.print(pressure*0.00750062);
  Serial.print("ммHg\tВысота:");
  Serial.print(altitude);
  Serial.println("м");
}
