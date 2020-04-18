//
// Created by Alexander Galilov on 10.04.2020.
// E-Mail: alexander.galilov@gmail.com 

#ifndef HARDWARE_H
#define HARDWARE_H

#include "main.h"
#include "RotaryEncoder.h"
#include "GyverButton.h"
#include "Led.h"
#include "RTClib.h"

class HardwareClass {
public:
    HardwareClass(int encoderDataPin, int encoderClkPin,
             int encoderBtnPin,
             int ledDataPin, int ledClkPin, int ledClkCsPin);

    void setup();

    void tick();

    inline RotaryEncoder &getEncoder() { return _encoder; }

    inline GButton &getButton() { return _button; }

    inline Led &getLed() { return _led; }

    inline PCF8563 &getRtc() {return _rtc;}

    int getEncoderPosition(int min, int max);

private:
    RotaryEncoder _encoder;
    GButton _button;
    Led _led;
    PCF8563 _rtc;
};

extern HardwareClass Hardware;

#endif //HARDWARE_H
