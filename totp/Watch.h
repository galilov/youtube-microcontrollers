//
// Created by Alexander Galilov on 12.04.2020.
// E-Mail: alexander.galilov@gmail.com 

#ifndef WATCH_H
#define WATCH_H

#include <Arduino.h>
#include "Hardware.h"
#include "Storage.h"

class WatchClass {
    static const unsigned long BLINK_PERIOD = 500;
    enum State {
        INITIAL, TIME
    };
public:
    WatchClass();

    void reset();

    bool tick();

private:
    State _state;
    char _buf[20];
    Storage::UTC _utc;
    DateTime _dt;
    unsigned long _prevTsBlink, _prevTs;
    unsigned long _nextUpdateDelta;

    void _initValues();

    void _updateTimeDisplay();
};

extern WatchClass Watch;

#endif //WATCH_H
