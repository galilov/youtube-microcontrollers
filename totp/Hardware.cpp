//
// Created by Alexander Galilov on 10.04.2020.
// E-Mail: alexander.galilov@gmail.com 

#include "Hardware.h"
#include <Keyboard.h>

HardwareClass::HardwareClass(int encoderDataPin, int encoderClkPin,
                   int encoderBtnPin,
                   int ledDataPin, int ledClkPin, int ledClkCsPin) :
        _encoder(encoderDataPin, encoderClkPin),
        _button(encoderBtnPin),
        _led(ledDataPin, ledClkPin, ledClkCsPin) {
}

int HardwareClass::getEncoderPosition(int min, int max) {
    int pos = _encoder.getPosition();
    if (pos > max) {
        pos = min;
        _encoder.setPosition(pos);
    } else if (pos < min) {
        pos = max;
        _encoder.setPosition(pos);
    }
    return pos;
}

void HardwareClass::setup() {
    HID(); // workaround to prevent linker from  "Keyboard.cpp:310: undefined reference to `HID()'" in CLion
    Serial.begin(9600);
    Keyboard.begin();
    _led.setup();
    _rtc.begin();
    if (!_rtc.isrunning()) {
        // following line sets the RTC to the date & time this sketch was compiled
        _rtc.adjust(DateTime(__DATE__, __TIME__));
    }
}

void HardwareClass::tick() {
    _button.tick();
    _encoder.tick();
}

HardwareClass Hardware(ENCODER_DT_PIN, ENCODER_CLK_PIN,
                   ENCODER_BTN_PIN,
                   LED_DATA_PIN, LED_CLK_PIN, LED_CS_PIN);
