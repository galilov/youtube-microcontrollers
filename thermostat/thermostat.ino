#define HEATER_PIN  6     // номер пина, к которому подключен нагреватель
#define NTC_PIN     A0    // номер A-пина, к которому подключен терморезистор и резистор R делителя.
#define R           9810  // сопротивление резистора в делителе, к которому подключен терморезистор NTC, Ohm
#define V           5.05   // напряжение питания на делителе, V
#define T0C         273.15 // температура в Кельвинах, соответствующая 0С
// Я уже вычислил коэффициэнты упрощенного уравнения Стейнхарта-Харта для нашего NTC терморезистора:
// Смотри https://ru.wikipedia.org/wiki/%D0%A3%D1%80%D0%B0%D0%B2%D0%BD%D0%B5%D0%BD%D0%B8%D0%B5_%D0%A1%D1%82%D0%B5%D0%B9%D0%BD%D1%85%D0%B0%D1%80%D1%82%D0%B0_%E2%80%94_%D0%A5%D0%B0%D1%80%D1%82%D0%B0
// https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
#define NTC_A       0.00055794
#define NTC_B       0.000315624
#define NTC_C       -1.44813E-07



#define MAX_ANALOG_LEVEL   1023 // Для Arduino Uno и др. на Atmega328p 
#define TARGET_TEMPERATURE 50   // целевая температура, C
#define DEVIATION          0.5  // допустимое отклонение, C

int heaterState;
void setup() {
  Serial.begin(115200);
  pinMode(HEATER_PIN, OUTPUT);
  digitalWrite(HEATER_PIN, LOW);
  heaterState = LOW;
}

float getNtcVoltage(int digitizedUntc) {
  return digitizedUntc * V / MAX_ANALOG_LEVEL;
}
float getNtcResistance(float Untc) {
  //+V o------+
  //      ^   |                   Untc      NTC
  //      |   Z                   ------- = ---,
  // Ur   |   Z R                 Ur         R
  //      |   Z
  //      v   |                   NTC = Untc * R / Ur,
  //          o---> NTC_PIN       Ur = Untc - V,
  //      ^   |                   NTC = Untc * R / (V - Untc)
  //      |   Z
  // Untc |   Z NTC
  //      |   Z
  //      v   |
  //GND-------+
  //
  //

  return R * Untc / (V - Untc);
}

float getTemperatureC(float ntcResistance) {
  // используем упрощенный расчет по уравнению Стейнхарта-Харта
  // 1/T = NTC_A + NTC_B * ln(ntc) + NTC_C * (ln(ntc)^3)
  // где T - температура в Кельвинах,
  // ln(ntc) - натуральный логарифм от сопротивления терморезистора
  double lnNtc = log(ntcResistance); // натуральный логарифм
  return 1.0 / (NTC_A + NTC_B * lnNtc + NTC_C * lnNtc * lnNtc * lnNtc) - T0C;
}

void loop() {
  Serial.println("====== loop ======");
  
  int digitizedUntc = analogRead(NTC_PIN);
  Serial.print("NTC ADC=");
  Serial.println(digitizedUntc);

  float Untc = getNtcVoltage(digitizedUntc);
  Serial.print("NTC U=");
  Serial.print(Untc);
  Serial.println(" Вольт");

  float ntcResistance = getNtcResistance(Untc);
  Serial.print("NTC R=");
  Serial.print(ntcResistance);
  Serial.println(" Ом");
  
  float t = getTemperatureC(ntcResistance);
  Serial.print("t=");
  Serial.print(t);
  Serial.println(" C");
  
  if (t < TARGET_TEMPERATURE - DEVIATION && heaterState != HIGH) {
    Serial.println("ВКЛЮЧАЕМ НАГРЕВАТЕЛЬ!!!");
    digitalWrite(HEATER_PIN, HIGH);
    heaterState = HIGH;
  } else if (t > TARGET_TEMPERATURE + DEVIATION && heaterState != LOW) {
    Serial.println("ВЫКЛЮЧАЕМ НАГРЕВАТЕЛЬ");
    digitalWrite(HEATER_PIN, LOW);
    heaterState = LOW;
  }
  delay(1000);
}
