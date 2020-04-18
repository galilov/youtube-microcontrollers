//
// Created by Alexander Galilov on 09.04.2020.
// E-Mail: alexander.galilov@gmail.com 

#ifndef __LED_H__
#define __LED_H__

#include <Arduino.h>
#include "LedControl.h"

class Led {
private:
    LedControl _lc;
public:
    Led(int dataPin, int clkPin, int csPin):
    _lc(dataPin, clkPin, csPin, 1) {
        //
    }

    void setup() {
        _lc.shutdown(0, false);
        /* Set the brightness to a medium values */
        _lc.setIntensity(0, 1);
        /* and clear the display */
        _lc.clearDisplay(0);
    }

    inline void clearDisplay() {
        _lc.clearDisplay(0);
    }

    void print(const char* data) {
        int len = strlen(data);
        int nPoints = 0;
        for (int8_t i = 0; i < len; i++) {
            if (data[i] == '.') nPoints++;
        }

        char prev;
        int offset = 0, targetPos;
        for (int8_t i = len - 1; i >= 0; --i) {
            char c = data[len - 1 - i];
            bool dp;
            if (c == '.') {
                dp = true;
                offset++;
            } else {
                dp = false;
            }
            targetPos = i + offset - nPoints;
            setChar(targetPos, dp ? prev : c, dp);
            prev = c;
        }
        for (int8_t j = 7; j >= len - nPoints; --j) {
            setChar(j, ' ', false);
        }
    }

    void setChar(int pos, char c, bool dp) {
        if (c == '@') {
            // print out "degree" symbol
            _lc.setRow(0, pos, 99);
        } else if (c == 'R' || c == 'r') {
            _lc.setRow(0, pos, 0b00000101);
        } else if (c == 'O') {
            _lc.setChar(0, pos, 0, dp);
        } else if (c == 'C') {
            _lc.setRow(0, pos, 0b01001110);
        }else if (c == 'U' || c == 'u') {
            _lc.setRow(0, pos, 0b00111110);
        } else if (c == 'o') {
            _lc.setRow(0, pos, 0b00011101);
        } else if (c == 'T' || c == 't') {
            _lc.setRow(0, pos, 0b00001111);
        } else {
            _lc.setChar(0, pos, c, dp);
        }
    }

    static inline bool needBlink(bool &needUpdate, unsigned long blinkPeriod, unsigned long& prevTs) {
        static bool blinkFlag = false;
        auto ts = millis();
        needUpdate = ts - prevTs > blinkPeriod || ts < prevTs;
        if (needUpdate) {
            prevTs = ts;
            blinkFlag = !blinkFlag;
        }
        return blinkFlag;
    }
};

#endif