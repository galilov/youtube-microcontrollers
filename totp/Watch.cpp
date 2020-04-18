//
// Created by Alexander Galilov on 12.04.2020.
// E-Mail: alexander.galilov@gmail.com 

#include "Watch.h"
#include "Storage.h"

WatchClass::WatchClass() : _state(INITIAL) {

}

void WatchClass::reset() {
    _state = INITIAL;
}

bool WatchClass::tick() {
    switch (_state) {
        case INITIAL:
            _initValues();
            _state = TIME;
            _updateTimeDisplay();
            break;
        case TIME:
            _updateTimeDisplay();
            break;
    }
}

void WatchClass::_initValues() {
    _prevTsBlink = 0;
    _prevTs = 0;
    _nextUpdateDelta = 0;
    Storage::getUtc(_utc);
}

void WatchClass::_updateTimeDisplay() {
    bool needUpdate;
    bool blinkFlag = Led::needBlink(needUpdate, BLINK_PERIOD, _prevTsBlink);
    auto ts = millis();

    if (ts - _prevTs > _nextUpdateDelta || _prevTs > ts) {
        _dt = Hardware.getRtc().now();
        _nextUpdateDelta = 60 - _dt.second();
        _prevTs = ts;
        needUpdate = true;
    }

    if (needUpdate) {
        TimeDelta td(0, abs(_utc.hours), _utc.minutes, 0);
        DateTime dtLocal;
        if (_utc.hours < 0) {
            dtLocal = (_dt - td);
        } else {
            dtLocal = (_dt + td);
        }
        if (blinkFlag) {
            sprintf(_buf, "%02u-%02u", dtLocal.hour(), dtLocal.minute());
        } else {
            sprintf(_buf, "%02u %02u", dtLocal.hour(), dtLocal.minute());
        }
        Hardware.getLed().print(_buf);
    }
}

WatchClass Watch;
