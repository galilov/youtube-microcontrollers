//
// Created by Alexander Galilov on 10.04.2020.
// E-Mail: alexander.galilov@gmail.com 

#include "RTCConfigurator.h"

RTCConfiguratorClass::RTCConfiguratorClass() :
        _state(INITIAL),
        _utcFormat("UTC %02d.%02u"),
        _utcFormatHoursOff("UTC   .%02u"),
        _utcFormatMinutesOff("UTC %02d.  "),
        _dateFormat("%04u.%02u.%02u"),
        _dateFormatYearsOff("    .%02u.%02u"),
        _dateFormatMonthsOff("%04u.  .%02u"),
        _dateFormatDaysOff("%04u.%02u.  "),
        _timeFormat("%02u-%02u-%02u"),
        _timeFormatHoursOff("  -%02u-%02u"),
        _timeFormatMinutesOff("%02u-  -%02u"),
        _timeFormatSecondsOff("%02u-%02u-  ") {
}

void RTCConfiguratorClass::reset() {
    _state = INITIAL;
}

bool RTCConfiguratorClass::tick() {
    if (Hardware.getButton().isHolded()) {
        Hardware.getButton().resetStates();
        _state = INITIAL;
        return true;
    }
    switch (_state) {
        case INITIAL:
            _initValues();
            break;
        case UTC_HOUR_OFFSET:
            _adjustUtcHourOffset();
            break;
        case UTC_MINUTE_OFFSET:
            _adjustUtcMinuteOffset();
            break;
        case YEAR:
            _adjustYear();
            break;
        case MONTH:
            _adjustMonth();
            break;
        case DAY:
            _adjustDay();
            break;
        case HOUR:
            _adjustHour();
            break;
        case MINUTE:
            _adjustMinute();
            break;
        case SECOND:
            return _adjustSecond();
    }
    return false;
}

void RTCConfiguratorClass::_updateDateDisplay() {
    bool needUpdate;
    bool blinkFlag = Led::needBlink(needUpdate, BLINK_PERIOD, _prevTs);

    if (!needUpdate && _prevYear != _year) {
        _prevYear = _year;
        needUpdate = true;
    }

    if (!needUpdate && _prevMonth != _month) {
        _prevMonth = _month;
        needUpdate = true;
    }

    if (!needUpdate && _prevDay != _day) {
        _prevDay = _day;
        needUpdate = true;
    }

    if (needUpdate) {
        if (blinkFlag) {
            switch (_state) {
                case YEAR:
                    sprintf(_buf, _dateFormatYearsOff, _month, _day);
                    break;
                case MONTH:
                    sprintf(_buf, _dateFormatMonthsOff, _year, _day);
                    break;
                case DAY:
                    sprintf(_buf, _dateFormatDaysOff, _year, _month);
                    break;
            }
        } else {
            sprintf(_buf, _dateFormat, _year, _month, _day);
        }
        Hardware.getLed().print(_buf);
    }
}

void RTCConfiguratorClass::_updateUtcDisplay() {

    bool needUpdate;
    bool blinkFlag = Led::needBlink(needUpdate, BLINK_PERIOD, _prevTs);

    if (!needUpdate && _prevUtc.hours != _utc.hours) {
        _prevUtc.minutes = _utc.minutes;
        needUpdate = true;
    }

    if (!needUpdate && _prevUtc.minutes != _utc.minutes) {
        _prevUtc.hours = _utc.hours;
        needUpdate = true;
    }

    if (needUpdate) {
        if (blinkFlag) {
            switch (_state) {
                case UTC_HOUR_OFFSET:
                    sprintf(_buf, _utcFormatHoursOff, _utc.minutes);
                    break;
                case UTC_MINUTE_OFFSET:
                    sprintf(_buf, _utcFormatMinutesOff, _utc.hours);
                    break;
            }
        } else {
            sprintf(_buf, _utcFormat, _utc.hours, _utc.minutes);
        }
        Hardware.getLed().print(_buf);
    }
}

void RTCConfiguratorClass::_updateTimeDisplay() {
    bool needUpdate;
    bool blinkFlag = Led::needBlink(needUpdate, BLINK_PERIOD, _prevTs);

    if (!needUpdate && _prevHour != _hour) {
        _prevHour = _hour;
        needUpdate = true;
    }

    if (!needUpdate && _prevMinute != _minute) {
        _prevMinute = _minute;
        needUpdate = true;
    }

    if (!needUpdate && _prevSecond != _second) {
        _prevSecond = _second;
        needUpdate = true;
    }

    if (needUpdate) {
        if (blinkFlag) {
            switch (_state) {
                case HOUR:
                    sprintf(_buf, _timeFormatHoursOff, _minute, _second);
                    break;
                case MINUTE:
                    sprintf(_buf, _timeFormatMinutesOff, _hour, _second);
                    break;
                case SECOND:
                    sprintf(_buf, _timeFormatSecondsOff, _hour, _minute);
                    break;
            }
        } else {
            sprintf(_buf, _timeFormat, _hour, _minute, _second);
        }
        Hardware.getLed().print(_buf);
    }
}

void RTCConfiguratorClass::_adjustUtcHourOffset() {
    _utc.hours = Hardware.getEncoderPosition(-24, 24);
    if (Hardware.getButton().isClick()) {
        _state = UTC_MINUTE_OFFSET;
        Hardware.getEncoder().setPosition(_utc.minutes);
    }
    _updateUtcDisplay();
}

void RTCConfiguratorClass::_adjustUtcMinuteOffset() {
    _utc.minutes = Hardware.getEncoderPosition(0, 59);
    if (Hardware.getButton().isClick()) {
        Storage::putUtc(_utc);
        _state = YEAR;
        Hardware.getEncoder().setPosition(_year - 2000);
    }
    _updateUtcDisplay();
}

void RTCConfiguratorClass::_adjustYear() {
    _year = 2000 + Hardware.getEncoderPosition(0, 100);
    if (Hardware.getButton().isClick()) {
        _state = MONTH;
        Hardware.getEncoder().setPosition(_month);
    }
    _updateDateDisplay();
}

void RTCConfiguratorClass::_adjustMonth() {
    _month = Hardware.getEncoderPosition(1, 12);
    if (Hardware.getButton().isClick()) {
        _state = DAY;
        Hardware.getEncoder().setPosition(_day);
    }
    _updateDateDisplay();
}

void RTCConfiguratorClass::_adjustDay() {
    _day = Hardware.getEncoderPosition(1, 31);
    if (Hardware.getButton().isClick()) {
        _state = HOUR;
        Hardware.getEncoder().setPosition(_hour);
    }
    _updateDateDisplay();
}

void RTCConfiguratorClass::_adjustHour() {
    _hour = Hardware.getEncoderPosition(0, 23);
    if (Hardware.getButton().isClick()) {
        _state = MINUTE;
        Hardware.getEncoder().setPosition(_minute);
    }
    _updateTimeDisplay();
}

void RTCConfiguratorClass::_adjustMinute() {
    _minute = Hardware.getEncoderPosition(0, 59);
    if (Hardware.getButton().isClick()) {
        _state = SECOND;
        Hardware.getEncoder().setPosition(_second);
    }
    _updateTimeDisplay();
}

bool RTCConfiguratorClass::_adjustSecond() {
    _second = Hardware.getEncoderPosition(0, 59);
    if (Hardware.getButton().isClick()) {
        _state = INITIAL;
        DateTime dt(_year, _month, _day, _hour, _minute, _second);
        TimeDelta timeDelta(0, abs(_utc.hours), _utc.minutes, 0);
        Hardware.getRtc().adjust(_utc.hours < 0 ? dt + timeDelta : dt - timeDelta);
        return true;
    }
    _updateTimeDisplay();
    return false;
}

void RTCConfiguratorClass::_initValues() {
    _state = UTC_HOUR_OFFSET;
    _prevUtc.minutes = 255;
    _prevUtc.hours = 255;
    _prevYear = 0;
    _prevMonth = 255;
    _prevDay = 255;
    _prevHour = 255;
    _prevMinute = 255;
    _prevSecond = 255;
    _prevTs = 0;
    Storage::getUtc(_utc);
    Hardware.getEncoder().setPosition(_utc.hours);
    auto dt = Hardware.getRtc().now();
    _year = dt.year();
    _month = dt.month();
    _day = dt.day();
    _hour = dt.hour();
    _minute = dt.minute();
    _second = dt.second();
}

RTCConfiguratorClass RTCConfigurator;